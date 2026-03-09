# Keil MDK-ARM & Cypress FM4 Installation Guide

### B39SB Lab Exercise #1 — Windows 11

---

## Before You Begin

Make sure you have the following ready:

- A Windows 11 PC with internet access
- Your Cypress FM4 Pioneer board
- A USB cable (data cable, not charge-only)

---

## PART 1: Install Keil MDK (Latest Version)

1. Open your browser and go to: **https://www.keil.arm.com/mdk-community/**
2. Click **Download MDK**
3. Fill in the registration form completely:
    
    - First Name, Last Name
    - Email address
    - Country
4. Click **Submit** — the download will begin automatically
    
    - File will be named something like `MDK539.exe`
5. Once downloaded, right-click the installer and select **Run as Administrator**
6. Click **Next** on the welcome screen
7. Accept the licence agreement and click **Next**
8. Make the installation path as below instead of default also arm toolchain path is :
    
    ```
    C:\Keil_v5
    C:\Arm
    ```
    
    > ⚠️ Do NOT change this path — other steps depend on it
9. Fill in your name and organisation, then click **Next**
10. Click **Install** and wait for it to complete (this may take several minutes)
11. When prompted to install drivers, click **Yes**
12. Click **Finish** — the **Pack Installer** will open automatically.

---

## PART 2: Install Required Packs (Better to keep this window open instead open the FM_4 project file and install whatever file is prompted)

> The Pack Installer should still be open from Part 1. If not, go to **Project → Manage → Pack Installer** in Keil.

### Install the Cypress FM4 Device Pack

1. In the search box at the top, type:
    
    ```
    Cypress FM4
    ```
2. Find **Keil::FM4_DFP** in the results
3. Click **Install** next to it
4. Wait for the download and installation to complete ✅

### Install and Verify CMSIS Pack (Must Be Version 5.0.0) (Super important, uninstall all other versions)

1. In the search box type:
    
    ```
    CMSIS
    ```
2. Find **ARM::CMSIS** in the results
3. Check the version number carefully:
    
    - If it shows version **5.0.0** → click **Install**
    - If it shows a **higher version** → click the version dropdown and select **5.0.0** specifically
    - If it shows **Update** → do NOT update, install version 5.0.0 only
    
    > ⚠️ **Important:** The FM4 project requires ARM::CMSIS version **5.0.0** exactly. Installing a newer version will cause a component compatibility error when building.
4. Wait for installation to complete ✅
5. Close the Pack Installer

---

## PART 3: Install ARM Compiler Version 5 (AC5) (Super important)

> Newer versions of Keil no longer bundle AC5. It must be installed separately.

1. Go to: **https://developer.arm.com/tools-and-software/embedded/arm-compiler/downloads/version-5**
2. Sign in with your Arm account (created in Part 2)
3. Download **ARM Compiler 5.06 update 7 (build 960)** — choose the **Windows 32** version
    
    > If the ARM website is not working, try this direct link: **https://developer.arm.com/downloads/view/ACOMP5**
4. **Run the downloaded file (Very important)**
5. When asked for the installation location, set it to exactly:
    
    ```
    C:\Keil_v5\ARM\ARMCC
    ```
    
    > ⚠️ This exact path is critical. Do not install anywhere else.
6. Click **Install** and wait for it to finish
7. Click **Finish**

### Register AC5 in Keil (Super important add this)

8. Open **Keil µVision 5**
9. Go to **Project → Manage → Project Items**
10. Click the **Folders/Extensions** tab
11. Under **ARM Compiler Folders**, click the **"..."** browse button
12. Navigate to:

```
```
C:\Keil_v5\ARM\ARMCC

```
```

13. Click **Add** then **OK** to close

---

## PART 4: Open and Configure the FM4 Project

1. Navigate to your `fm4_folder` in File Explorer
2. Double-click `fm4_project.uvprojx` to open it in Keil µVision 5
3. Go to **Project → Options for Target** (or press **Alt+F7**)
4. Click the **Target** tab
5. In the **ARM Compiler** dropdown, select:
    
    ```
    Use ARM Compiler Version 5 (C:\Keil_v5\ARM\ARMCC)
    ```
    
    > ⚠️ Make sure it says **ARMCC** not **ARMCLANG**
6. Click **OK** to close Settings, then **OK** again to close Options

---

## PART 5: Build and Run the Project

1. Go to **Project → Rebuild All Target Files** (or press **F7**)
2. Watch the Build Output window at the bottom. A successful build looks like:
    
    ```
    "fm4_project.axf" - 0 Error(s), 0 Warning(s).
    Build Time Elapsed: 00:00:XX
    ```
3. Make sure your FM4 board is connected via USB



