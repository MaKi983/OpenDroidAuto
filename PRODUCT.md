# Product

<!-- impeccable:product-schema 1 -->

## Platform

android

## Users

Drivers and installers using legacy Android ARM32 multimedia head units, primarily Honda Connect units and compatible generic devices.

## Product Purpose

OpenDroidAuto turns a legacy head unit into an Android Auto receiver through USB AOAP or a phone hotspot. Success is a reliable, low-distraction connection that starts manually only after the transport is ready.

## Positioning

Unlike a generic Android Auto client, the product preserves support for Android 4.x Honda hardware while safely falling back when Honda/Fujitsu system services are unavailable.

## Operating Context

The interface is used in a landscape car display, often at 800x480, while the vehicle is stationary or being driven. A saved phone hotspot may be used for wireless Android Auto.

## Capabilities and Constraints

- Keep Android 4.x, `armeabi-v7a`, target SDK 15, and NDK 17.2 compatibility.
- USB starts manually from the dashboard after Android grants device access; Android system permissions cannot be bypassed without privileged installation.
- USB preparation runs asynchronously with bounded retries and an AOAP re-enumeration watchdog; it must never block the Android 4.x UI thread.
- Release APKs use one stable certificate so updates preserve package identity and the Android USB default association.
- Start in a high-contrast light theme and persist the driver-selected light or dark appearance across restarts.
- Use Portuguese resources for any Portuguese system locale and English as the fallback for every other language.
- Wi-Fi uses a hotspot already saved by the Android system; Wi-Fi Direct and in-app password provisioning are out of scope.
- Honda integration stays available when compatible and must not crash generic devices.

## Brand Commitments

OpenDroidAuto remains recognisable as an automotive utility: concise, high-contrast, calm, and legible at a glance.

## Evidence on Hand

The app ships a legacy logo and current connection assets in `app/src/main/res/drawable/`. Hardware-specific behavior requires validation on an actual head unit and phone.

## Product Principles

- Connect first; ask only when the operating system requires it.
- Make connection state and recovery obvious without distracting the driver.
- Preserve the working Honda path while degrading safely elsewhere.
- Projection lifecycle changes must be based on the proven manual-start implementation; transport hardening stays outside that lifecycle unless hardware evidence requires otherwise.
- Prefer bounded retries and clear recovery over hidden loops.

## Accessibility & Inclusion

Use landscape-friendly high contrast, readable type, and at least 48dp touch targets for actions available while parked.
