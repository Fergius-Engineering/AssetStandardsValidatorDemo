# Asset Standards Validator — Demo

Validates and enforces asset naming, folder structure, and quality standards inside Unreal Engine 5. Catches bad assets before they reach the project or the pipeline.

Inspired by [Allar's UE5 Style Guide](https://github.com/Allar/ue5-style-guide).

---

## ⚠️ Demo limitations

- **Scans up to 200 assets per run** — the most recently modified assets in scope. Single-asset validation (right-click → Validate) has no cap.
- **Batch Fix is limited to 5 uses per editor session** — resets on restart. Fixing one asset at a time is unlimited.
- **CI commandlet is disabled** — `ASVCommandlet` exits with an error in demo builds.
- **No source code** — custom validators (C++/Blueprint) require the full version.
- **Windows only** — full version supports Windows, Linux, and Mac.

---

## Install

1. Go to [**Releases**](https://github.com/Fergius-Engineering/AssetStandardsValidatorDemo/releases) and download the zip for your UE version: `AssetStandardsValidator_Demo_{ue}_{ver}.zip`
2. Extract the zip — you'll get an `AssetStandardsValidator` folder
3. Copy it to `UE_5.x/Engine/Plugins/Marketplace/AssetStandardsValidator/`
4. Open your project, go to **Edit → Plugins**, search for **Asset Standards Validator**, enable it, and restart the editor

> **UE versions:** 5.0 · 5.1 · 5.2 · 5.3 · 5.4 · 5.5 · 5.6 · 5.7  
> **Editor-only plugin.** Not included in packaged builds.

---

## What's included

- **Naming Convention** — prefix, suffix, junk names, non-ASCII
- **Folder Structure** — wrong location, bad path format, Developers/ check
- **Texture** — power-of-two, max size, sRGB, compression, LOD group
- **Blueprint** — variable naming, bool prefix, tooltips, compilation errors
- **Static Mesh** — collision, LOD, Nanite policy
- **Asset Health** — stale redirectors
- **Auto-fix** — renames, folder moves, texture property fixes, batch fix (5 uses/session in demo)
- **Batch scan** — whole project or single folder
- **Panel** — sortable results, search, filters, double-click to navigate
- **In-editor docs** — full reference, searchable, works offline
- **300+ automated tests** — validators, auto-fix, and edge cases covered on every build

---

## Demo vs Full

| Feature | Demo | Full |
|---------|------|------|
| All validators | ✅ | ✅ |
| Auto-fix (single asset) | ✅ | ✅ |
| Batch fix | ⚠️ 5 uses/session | ✅ |
| Batch scan | ⚠️ 200 assets/run | ✅ |
| In-editor docs | ✅ | ✅ |
| Report export (HTML/JSON/CSV) | ✅ | ✅ |
| CI commandlet | ❌ | ✅ |
| Custom validators (C++/BP) | ❌ | ✅ |
| Source code | ❌ | ✅ |
| Platforms | Windows only | Windows + Linux + Mac |

---

## Quick start

1. Open the plugin panel: **Tools → Asset Standards Validator → Open ASV Panel**
2. Click **Run Full Audit** in the toolbar
3. Wait for scan to finish — results appear in the panel
4. Click any result to select the asset in the Content Browser
5. Click **Auto-fix** on a result (or right-click → Fix) to apply the fix

![ASV Panel — validation results after a full audit](docs/screenshots/asv-panel-results.png)

![Asset overlay — violation badges visible directly in the Content Browser](docs/screenshots/asv-asset-overlay.png)

---

## Batch Fix (full version)

Fix an entire filtered rule at once — no need to click through each asset individually.

**Flow:**
1. Run a full audit and wait for results
2. Use the **Rule** filter dropdown to narrow results to a single rule (e.g. `missing_prefix`)
3. The **Fix All** button in the toolbar activates when 2–200 fixable results are visible
4. Click **Fix All** — a dialog opens showing all assets grouped by folder
5. Review the list, uncheck any assets you want to skip; conflicting assets are already flagged ⚠
6. Click **Fix Selected** — progress updates per row in real time
7. When done, the panel refreshes and a summary toast appears

![Batch Fix dialog — review assets and apply fixes in bulk](docs/screenshots/asv-batch-fix-dialog.png)

> Demo allows up to 5 batch fix uses per editor session. The button shows remaining uses in the tooltip. Resets on editor restart.

---

## Validators and rules

### Naming Convention

Checks asset names against the [Allar UE5 Style Guide](https://github.com/Allar/ue5-style-guide).

| Rule ID | What it checks | On by default | Config property |
|---------|---------------|:-------------:|----------------|
| `missing_prefix` | Asset has required class prefix (e.g. `BP_`, `T_`) | ✅ | `bCheckPrefix` |
| `duplicate_prefix` | Asset name does not have the prefix repeated | ✅ | `bCheckPrefix` |
| `missing_suffix` | Asset has required class suffix when defined | ✅ | `bCheckSuffix` |
| `missing_allowed_suffix` | World assets use allowed level suffixes | ✅ | `bCheckAllowedSuffix` |
| `name_pattern_mismatch` | Name matches the allowed pattern regex | ✅ | `bCheckNamePattern` |
| `non_ascii_name` | No non-ASCII characters in name | ✅ | `bCheckNonAsciiName` |
| `junk_name` | Name is not a placeholder (New, Temp, Default…) | ✅ | `bCheckJunkName` |

Auto-fix: `missing_prefix`, `duplicate_prefix`, `missing_suffix`, `missing_allowed_suffix`, `junk_name` (renames asset).

---

### Folder Structure

Checks that assets are in the right folders and paths are well-formed.

| Rule ID | What it checks | On by default | Config property |
|---------|---------------|:-------------:|----------------|
| `wrong_folder` | Asset is in the correct folder for its class | ✅ | `bCheckWrongFolder` |
| `folder_not_pascal_case` | Each folder segment uses PascalCase | ✅ | `bCheckPascalCase` |
| `folder_contains_unicode` | No non-ASCII characters in folder path | ✅ | `bCheckUnicode` |
| `folder_disallowed_name` | No generic folder names (Assets, Meshes…) | ❌ | `bCheckDisallowedNames` |
| `folder_in_developers` | Asset is not inside the Developers/ folder | ✅ | `bCheckDevelopersFolder` |

`folder_disallowed_name` is off by default — the disallowed name list needs project configuration.

Auto-fix: `wrong_folder` (moves asset to correct folder).

---

### Texture

Checks texture properties from asset metadata — no full load required.

| Rule ID | What it checks | On by default | Config property |
|---------|---------------|:-------------:|----------------|
| `texture_not_power_of_two` | Width and height are both powers of two | ✅ | `bCheckPowerOfTwo` |
| `texture_exceeds_max_size` | Width and height within max size (default 8192) | ✅ | `bCheckMaxSize` |
| `texture_wrong_srgb` | sRGB flag matches texture type (color vs non-color) | ✅ | `bCheckSRGB` |
| `texture_wrong_compression` | Compression matches texture type | ✅ | `bCheckCompression` |
| `texture_wrong_group` | LOD group matches texture suffix | ✅ | `bCheckTextureGroup` |

Detection is based on name suffix: `_D` → color (sRGB on), `_N` → normal (sRGB off), etc.

Auto-fix: `texture_wrong_srgb`, `texture_wrong_compression`, `texture_wrong_group` (sets the correct property value).

---

### Blueprint

Checks Blueprint variable conventions and compilation status.

| Rule ID | What it checks | On by default | Config property |
|---------|---------------|:-------------:|----------------|
| `bp_bool_no_prefix` | Boolean variables start with `b` | ✅ | `bCheckBoolPrefix` |
| `bp_bool_is_pattern` | Avoid `bIsDead` — prefer `bDead` | ✅ | `bCheckBoolIsPattern` |
| `bp_var_not_pascal_case` | Variable uses PascalCase | ✅ | `bCheckPascalCase` |
| `bp_var_atomic_type_name` | Name does not include type (`Score` not `ScoreInt`) | ✅ | `bCheckAtomicTypeName` |
| `bp_editable_missing_tooltip` | Editable variables have tooltip text | ✅ | `bCheckEditableTooltips` |
| `bp_var_config_flag` | Variable does not use the Config flag | ✅ | `bCheckConfigFlag` |
| `bp_editable_missing_range` | Editable numeric variables have a range set | ✅ | `bCheckEditableRanges` |
| `bp_vars_uncategorized` | Editable variables are categorized (when ≥ 5) | ✅ | `bCheckUncategorizedVars` |
| `bp_var_redundant_context` | Variable name doesn't repeat the class name | ✅ | `bCheckRedundantContext` |
| `var_array_not_plural` | Array variables use plural names | ❌ | `bCheckArrayPlural` |
| `var_missing_type_name` | Struct/object variables include type name | ❌ | `bCheckComplexTypeName` |
| `blueprint_compile_error` | Blueprint has no compilation errors | ✅ | `bCheckCompilation` |
| `blueprint_compile_warning` | Blueprint has no compilation warnings | ✅ | `bCheckCompilation` |
| `blueprint_needs_compile` | Blueprint is up to date (not dirty) | ✅ | `bCheckCompilation` |

`var_array_not_plural` and `var_missing_type_name` are off by default — they produce many false positives without project-specific tuning.

Auto-fix: `bp_bool_no_prefix` (renames to add `b`), `bp_bool_is_pattern` (renames, removes `Is`/`Has`/`Can`), `bp_var_not_pascal_case` (renames to PascalCase).

---

### Static Mesh

Checks mesh quality settings. Requires loading the UStaticMesh asset.

| Rule ID | What it checks | On by default | Config property |
|---------|---------------|:-------------:|----------------|
| `mesh_no_collision` | Mesh has collision geometry | ✅ | `bCheckCollision` |
| `mesh_no_lods` | Mesh has LODs (for meshes over 5 000 triangles) | ✅ | `bCheckLODs` |
| `mesh_nanite_policy` | Nanite is enabled/disabled as required | ❌ | `bCheckNanite` |

`mesh_nanite_policy` is off by default — the policy (must enable / must disable / allow any) needs a project decision.

Auto-fix: `mesh_nanite_policy` (enables or disables Nanite per policy). Collision and LOD changes are manual.

---

### Asset Health

Checks general asset state. Reads from the Asset Registry — no loading required.

| Rule ID | What it checks | On by default | Config property |
|---------|---------------|:-------------:|----------------|
| `stale_redirector` | ObjectRedirector has been resolved | ✅ | `bCheckRedirectors` |

Auto-fix: `stale_redirector` (consolidates the redirector).

---

## Audit Report

Export a full HTML report after any scan: **Export Report → HTML**. Opens in your browser — health score, top violations by impact, breakdown by folder.

![HTML audit report — health score, violations by rule and folder](docs/screenshots/asv-html-report.png)

---

## Configuration

On first run the plugin creates a config DataAsset automatically: **`Content/Data/DA_ASV_AllarStyleGuide`**. Open it to enable or adjust checks — no setup required to get started.

**Project Settings → Plugins → Asset Standards Validator** — global settings: scan roots, exclude paths, logging.

![Checker config DataAsset — validators, triggers, and path filters](docs/screenshots/asv-checker-config.png)

### Enabling off-by-default rules

Rules marked ❌ in the tables above are off in the auto-created config. To enable one:

1. Open `Content/Data/DA_ASV_AllarStyleGuide` in the Content Browser
2. Expand the relevant validator in the **Validators** list
3. Toggle the property on

**Example — `mesh_nanite_policy`:**
1. Expand **ASVValidator_Mesh**
2. Set `bCheckNanite = true`
3. Set `NanitePolicy` to `MustEnable`, `MustDisable`, or `AllowAny`
4. Optionally adjust `MinTriCountForNanite` (default 10 000)

**Example — `folder_disallowed_name`:**
1. Expand **ASVValidator_FolderStructure**
2. Set `bCheckDisallowedNames = true`
3. Add folder names to `DisallowedFolderNames` (e.g. `Assets`, `Meshes`)

### Custom class prefixes

For project-specific asset types (e.g. `GA_` for Gameplay Abilities, `GE_` for Gameplay Effects):

1. Open `Content/Data/DA_ASV_AllarStyleGuide`
2. Expand **ASVValidator_NamingConvention** → under **Class Rules**, click **+**
3. Set **Class Picker** to the parent class (e.g. `UGameplayAbility`) — Blueprint subclasses match automatically
4. Set **Prefix** to `GA_`

For a simple class name → prefix mapping without inheritance, use **Default Class Prefixes** instead (a key/value map on the same validator).

---

## In-editor documentation

Full reference is available inside the editor: **Tools → Asset Standards Validator → Documentation**. Works offline, searchable, covers all validators, auto-fix, configuration, and reports.

![In-editor documentation — searchable reference, works offline](docs/screenshots/asv-docs-tab.png)

---

## Get full version

[**Asset Standards Validator on Fab →**](https://www.fab.com/sellers/Fergius%20Engineering)

Full version includes source code, packaging support, and the ability to write custom validators in C++ or Blueprint.

> **Before installing the full version:** remove the demo plugin first. Both versions share the same module name and will conflict if installed together. Delete or rename the `AssetStandardsValidator` folder in your project's `Plugins/` directory, then restart the editor before installing the full version.

---

## Bugs and questions

[Join Discord →](https://discord.gg/Zc7Y7nYrvz)

Please include your UE version and a brief description of what happened.
