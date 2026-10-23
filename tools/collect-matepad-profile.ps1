param(
    [string]$Adb = "adb",
    [string]$Serial = "",
    [string]$Output = "matepad-profile.txt"
)

$ErrorActionPreference = "Stop"

function Invoke-AdbRead {
    param([Parameter(ValueFromRemainingArguments = $true)][string[]]$Arguments)
    $prefix = @()
    if ($Serial) { $prefix = @("-s", $Serial) }
    & $Adb @prefix @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "ADB command failed: $($Arguments -join ' ')"
    }
}

$properties = @(
    "ro.product.manufacturer",
    "ro.product.brand",
    "ro.product.model",
    "ro.product.device",
    "ro.product.name",
    "ro.product.board",
    "ro.hardware",
    "ro.board.platform",
    "ro.build.fingerprint",
    "ro.build.display.id",
    "ro.build.id",
    "ro.build.version.incremental",
    "ro.build.version.release",
    "ro.build.version.sdk",
    "ro.build.version.security_patch",
    "ro.build.type",
    "ro.build.tags",
    "ro.build.characteristics",
    "ro.build.version.emui",
    "ro.config.hw_is_pad",
    "ro.product.cpu.abi",
    "ro.product.cpu.abilist",
    "ro.sf.lcd_density"
)

$lines = [System.Collections.Generic.List[string]]::new()
$lines.Add("# MatePad profile (read-only, identifiers excluded)")
$lines.Add("collected_at=$([DateTimeOffset]::Now.ToString('o'))")
$lines.Add("")
$lines.Add("[properties]")
foreach ($property in $properties) {
    $value = (Invoke-AdbRead shell getprop $property | Out-String).Trim()
    $lines.Add("$property=$value")
}

$lines.Add("")
$lines.Add("[display]")
$lines.AddRange([string[]](Invoke-AdbRead shell wm size | ForEach-Object { $_.ToString() }))
$lines.AddRange([string[]](Invoke-AdbRead shell wm density | ForEach-Object { $_.ToString() }))

$lines.Add("")
$lines.Add("[features]")
$featurePatterns = "android.hardware.screen|android.hardware.sensor|android.hardware.touchscreen|com.huawei|huawei"
$features = Invoke-AdbRead shell pm list features
$lines.AddRange([string[]]($features | Select-String -Pattern $featurePatterns | ForEach-Object { $_.Line }))

$lines.Add("")
$lines.Add("[cpu]")
$cpu = Invoke-AdbRead shell cat /proc/cpuinfo
$lines.AddRange([string[]]($cpu | Select-String -Pattern "Hardware|model name|Processor" | ForEach-Object { $_.Line } | Select-Object -Unique))

$lines.Add("")
$lines.Add("# Excluded: serial number, Android ID, IMEI, MAC addresses, accounts and user data.")
$lines | Set-Content -LiteralPath $Output -Encoding UTF8
Write-Host "Saved read-only profile to: $Output"
