# Digimon Digital Card Battle, USA (SLUS-01328)
#
# Plain values: tools/version.py reads EXE_NAME and DISK_DIR from here too.

# the executable, as it is on the disc
EXE_NAME := SLUS_013.28

# where the dumped disc is: the executable and P.DRV, which holds the overlays
DISK_DIR := disks/us

# the overlays the build splits out of P.DRV, links and checks: each one has
# its splat config, config/<version>/<name>.yaml
OVERLAYS := endseg evoseg kawseg openseg saiseg subseg sugseg
