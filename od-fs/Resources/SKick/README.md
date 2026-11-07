# SKick

SKick 3.46, a Kickstart soft-kicker by Pavel Troller (SinSoft), maintained by Jean-François Fabre
since version 3.44.

| File           | SHA-1                                      | Source                             |
|----------------|--------------------------------------------|------------------------------------|
| `skick346.lha` | `403bc01200e761900b5b80b56809805a1c65f955` | Aminet, `util/boot/skick346.lha`   |

FS-UAE uses the Kickstart relocation tables in the archive (`Kickstarts/*.RTB`), which WHDLoad
combines with the user's own Kickstart ROM images. FS-UAE Launcher installs these into
`Devs/Kickstarts/` for WHDLoad games: `kick33180.A500.RTB`, `kick34005.A500.RTB`,
`kick40068.A1200.RTB` and `kick40068.A4000.RTB`.

## License

From `SKick.doc`: "This archive may be freely redistributed, but only in totally unchanged state,
i.e. no files can be added, deleted, modified etc." The archive is therefore kept packed and
byte-identical to the Aminet release, and the files needed must be extracted from it when used.
