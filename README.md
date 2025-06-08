SvOlli's Little Audio Related Thingies
======================================

About
-----

"SvOlli's Little Audio Related Thingies", or "SLART" for short, is a collection
of tools I hacked together myself for myself. These tools, once complete, will
cover every aspect of handling audio files I came up with.

| Function              | Name          | Status  | Named After The Song By |
| --------------------- | ------------- | ------- | ----------------------- |
| Player                | Partyman      | beta    | Prince                  |
| CD Reader             | Stripped      | alpha   | Depeche Mode            |
| Tag Editor / Database | Rubberbandman | alpha   | Yello                   |
| Downloader            | Funkytown     | broken  | Lipps Inc.              |
| Playlist Management   | Karmadrome    | alpha   | Pop Will Eat Itself     |
| LIRC Client           | Creep         | unknown | Radiohead               |
| FreeDB Browser        | Notorious     | alpha   | Duran Duran             |
| Central App.          | Innuendo      | alpha   | Queen                   |
| Setup Wizard          | Sorcerer      | alpha   | Marilyn Martin          |

Funkytown could not be ported straight forward from Qt4 to Qt5. Since is was
not very useful anymore, it was taken out of the build process.

Creep has not been used for a long time, since remote did not function after
system upgrade.

It contains also some classes that are usable for other applications:
- a simple command line arguments handler
- Satellite: a simple interprocess communication class

Primary git repository is available at:
https://github.com/SvOlli/SLART
https://git.h8u.de/svolli/SLART

