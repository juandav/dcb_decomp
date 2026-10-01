# Digimon Digital Card Battle, Europe (SLES-03900)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the executable, as it is on the disc
EXE_NAME := SLES_039.00

# where the dumped disc is: the executable and P.DRV, which holds the overlays
DISK_DIR := disks/eu

# the overlays the build splits out of P.DRV, links and checks: each one has
# its splat config, config/<version>/<name>.yaml
OVERLAYS := endseg intseg kawseg nisseg saiseg subseg sugseg vssver openseg evoseg

# The source files this version builds (see mk/version/us.mk): none yet.
# Every binary is splat's assembly (config/eu/*.yaml), except VSSVER: not
# code but a Visual SourceSafe file, it stays one blob, splat's databin.
