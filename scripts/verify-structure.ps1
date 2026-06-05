# Verify repository structure (does not compile without course CodeGenerator.h).

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$parts = @("Part 1.c", "Part 2.c", "Part 3.c")
$ok = $true

foreach ($p in $parts) {
    $path = Join-Path $Root $p
    if (-not (Test-Path $path)) {
        Write-Host "MISSING: $p"
        $ok = $false
        continue
    }
    $content = Get-Content $path -Raw
    if ($content -notmatch '#include\s+"CodeGenerator\.h"') {
        Write-Host "WARN: $p does not include CodeGenerator.h"
        $ok = $false
    } else {
        Write-Host "OK: $p"
    }
}

if (-not $ok) { exit 1 }
Write-Host "Structure check passed (integration with course framework still required to build)."
