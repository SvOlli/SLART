\mainpage
# SvOlli's Little Audio Related Thingies

or SLART for short is a collection of tool hacked up to cover several
aspects listening to music and beyond, like ripping CDs etc.

The project consists of several components:
| Module Name            | Short Description                            |
| ---------------------- | -------------------------------------------- |
| \ref PartymanPage      | double deck audio player                     |
| \ref InnuendoPage      | central starting and information application |
| \ref RubberbandmanPage | tag editor                                   |
| \ref KarmadromePage    | playlist/group management                    |
| \ref UnderpassPage     | player for audio streams                     |
| \ref FunkytownPage     | downloading tool                             |
| \ref NotoriousPage     | freedb database browser                      |
| \ref StrippedPage      | ripper for audio CDs                         |
| \ref SorcererPage      | setup wizard                                 |
| \ref CreepPage         | remote control tool                          |
| \ref CommandLinePage   | command line arguments parser                |
| \ref CommonPage        | common subroutines library                   |
| \ref KryptonitePage    | downloading framework                        |
| \ref MagicPage         | audio decoder/encoder interface              |
| \ref SatellitePage     | interprocess communication                   |



\page PartymanPage Partyman

\section Satellite Communication
| Code    | Action                                   | Parameters                           |
| ------- | ---------------------------------------- | ------------------------------------ |
| **P0Q** | En<b>q</b>ueue track                     | filename                             |
| **P0A** | Pl<b>a</b>y / p<b>a</b>use               |                                      |
| **P0S** | <b>S</b>top                              |                                      |
| **P0N** | Skip to <b>n</b>ext track                |                                      |
| **P0R** | <b>R</b>e-send last "p0p" status message |                                      |
| **P0C** | <b>C</b>heck:                            | 1: unwanted, 2: favorite, 0: uncheck |
| **P0T** | Change to <b>t</b>ab                     | count started with 0 from left       |
|   p0p   | <b>P</b>laying track                     | absolute path + filename             |
|   p0s   | <b>S</b>topped                           |                                      |
|   p0n   | Skipping to <b>n</b>ext                  |                                      |
|   p0a   | P<b>a</b>used                            |                                      |
|   p0u   | Database <b>u</b>pdated                  |                                      |
|   p0c   | <b>C</b>onfiguration updated             |                                      |



\page InnuendoPage Innuendo

\section Satellite Communication
| Code    | Action                               | Parameters  |
| ------- | ------------------------------------ | ----------- |
| **I0L** | <b>L</b>og (write output to console) | text to log |



\page RubberbandmanPage Rubberbandman

\section Satellite Communication
| Code    | Action                  | Parameters |
| ------- | ----------------------- | ---------- |
| **R0T** | Run database load test  |            |
|   r0u   | Database <b>u</b>pdated |            |
\todo update parameters for R0T



\page KarmadromePage Karmadrome

\section Satellite Communication
| Code    | Action                                    | Parameters               |
| ------- | ----------------------------------------- | ------------------------ |
| **K0A** | <b>A</b>dd/remove current track to folder | foldername               |
| **K0E** | <b>E</b>xport folder from file            | foldername, filename.m3u |
| **K0I** | <b>I</b>mport folder from file            | foldername, filename.m3u |
|   k0u   | Database <b>u</b>pdated                   |                          |



\page UnderpassPage Underpass

\section Satellite Communication



\page FunkytownPage Funkytown

\section Satellite Communication
| Code    | Action              | Parameters |
| ------- | --------------------| ---------- |
| **U0A** | Pl<b>a</b>y         |            |
| **U0S** | <b>S</b>top         |            |
| **U0N** | <b>N</b>ext station |            |
|   u0p   | <b>P</b>laying      |            |
|   u0s   | <b>S</b>topped      |            |



\page NotoriousPage Notorious



\page StrippedPage Stripped

\section Satellite Communication
| Code    | Action                  | Parameters        |
| ------- | ----------------------- | ----------------- |
|   s0d   | Done with ripping track | complete filename |



\page SorcererPage Sorcerer



\page CreepPage Creep

A small command line only tool indended to be run as a daemon. Using the
.lircrc configuration file, it converts remote control commands to
\ref Satellite messages.



\page CommandLinePage CommandLine



\page CommonPage Common



\page KryptonitePage Kryptonite



\page MagicPage Magic



\page SatellitePage Satellite



\section Generic Satellite Communication
| Code    | Action                | Parameters                    |
| ------- | ----------------------| ----------------------------- |
| **CFG** | re-read configuration |                               |
| **PNG** | ping request          |                               |
| **SHT** | screenshot            | appname, widgetname, filename |
|   png   | ping reply            | appname                       |
