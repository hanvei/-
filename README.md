# Android Space Demo

Personal Android demonstration integrating existing open-source components.

## Current stage

Island and its Deagle dependency have been imported as a build baseline. The first CI run verifies the upstream application before feature changes. Shelter features and SD-card workflows are not integrated yet. A successful build is not evidence of successful device provisioning or compatibility with a particular phone.

## Build

GitHub: Actions → Android demo → Run workflow. When the job succeeds, download `android-space-demo-debug` from Artifacts and unzip the APK. This is a debug build for testing.

Local: JDK 11 and Android SDK platforms 27/30 are required. From `island`, run `./gradlew :assembly:assembleCompleteDebug` (Windows: `gradlew.bat :assembly:assembleCompleteDebug`).

This baseline retains Island's original application ID. Do not install it as an update over an existing Island-managed profile: it has a different signing key. A distinct application identity is planned before the integrated demo is ready for device use.

## Intended integration

- Reuse Island's profile management, Mainland management and existing file shuttle.
- Port Shelter's screen-off auto-freeze behavior into the same management engine.
- Add user-selected external-storage file import/export, including SD-card folders where Android permits access.
- Do not promise that the manager or an entire work profile can move to an SD card. Android system and device constraints must be verified separately.

## Sources

- Island: https://github.com/oasisfeng/island (master snapshot downloaded 2026-10-19; queried HEAD d63538212a9f417c180bdb9258c0f5461de53c27).
- Deagle: https://github.com/oasisfeng/deagle (master snapshot downloaded 2026-10-19).
- Shelter reference: https://github.com/PeterCxy/Shelter (downloaded separately; no source ported yet).

Upstream license files and attribution are retained in their source directories.
