# OpenDroidAuto

## Debugging

For debugging, hondaIntegrationEnabled() must be false, otherwise you will get an error on startup

## How to build

./gradlew :common:assemble
./gradlew assembleRelease

## Generate apk

./gradlew assembleRelease
You will find the apk in app/build/outputs/apk/release
