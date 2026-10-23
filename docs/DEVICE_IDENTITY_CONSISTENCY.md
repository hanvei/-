# Device identity consistency pass

This pass improves the internal consistency of the WGR-W09 proof-of-concept. It does not copy a physical tablet's serial number, Android ID, IMEI, MEID, attestation key, or hardware key, and it does not modify the guest WeChat package.

## Evidence and change

- `DeviceSpoofManager.java` is the single source for the private per-user serial and Android ID. Older DBY-W09 and Mate X6 experimental profiles are migrated to the WGR-W09 public model fields while retaining their existing private identifiers and virtual-user data.
- `DeviceBuildSpoofer.java` exposes the profile serial through `Build.SERIAL`.
- `IDeviceIdentifiersPolicyProxy.java` now returns the same profile serial from `getSerialForPackage()`. Previously it returned an unrelated 32-character MD5 of the host package name.
- The WGR-W09 reference is a Wi-Fi tablet, so telephony identifier calls now return no IMEI or MEID instead of fabricated values.
- `ISettingsProviderProxy.java` continues to return the profile's stable private Android ID.

These changes only remove contradictions between public Android APIs. They do not suppress WeChat security checks and do not claim that server-side acceptance will change.
