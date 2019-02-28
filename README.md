# Introduction

Welcome to the Community Update Project for Gallery 2, a web based software product that lets you manage your photos on your own website. This effort aims towards the development of community backed updates to Gallery 2.

Official development of Gallery 2 has stopped in favour of Gallery v3 but as Gallery v3 never really quite got to the same level, Gallery 2 remained for many, in testament to the work of the development team, the most powerful, most fully featured and most flexible and agile choice.

In any case, development of Gallery v3 soon came to a halt and with that, an end to the Official Gallery Project. The aim of this effort is to add enhancements and fixes to the Gallery 2 code so that its several loyal fans can continue using this awesome piece of software engineering.

# Supported PHP Versions

This effort seeks to provide a version of Gallery 2 that supports PHP 5.3.3 to PHP 7.3.3.

In more detail:

  - Absolute Minimum PHP Version = PHP 5.3.3 (**Should function** but will be significantly slower and require significant additional server resources)
  - Acceptable Minimum PHP Version = PHP 5.6.0 (**Will function** but will be significantly slower and require additional server resources)
  - Desirable Minimum PHP Version = PHP 7.1.0 (**Will function** but will require some additional server resources)
  - Suggested Minimum PHP Version = PHP 7.2.0 (**Will function** with minimal additional server resources)
  - Optimal PHP Version = PHP 7.3.x (**Will function** without speed or server issues)

# Choosing a package

You can choose from one of the following branches:

The easiest way is simply to download and install [The Gallery 2 Pre-Installer](https://github.com/dakanji/G2Project-preinstaller).

This is a small script file that can be easily uploaded to your site and when run by navigating to the file, give a recommendation of which Gallery 2 package to install, transfer this or any other selected Gallery 2 package to your site, extract the files and provide a link to install Gallery 2.

Alternatively, you can take a look at [The Release Archives](https://github.com/dakanji/G2Project/releases) and decide for yourself.

For more advanced users interested in the Github branches, these are set out as follows:

- **[Master Branch](https://github.com/dakanji/G2Project/tree/master)** - This branch contains the latest stable code and is always deployable. Relatively few updates, features or bug fixes will have been applied and it will be the closest branch to the last official release by the Gallery Project Team. However, this also means it has not yet been updated for current PHP Versions. The current file set can be [DOWNLOADED HERE](https://github.com/dakanji/G2Project/archive/master.zip).
- **[Beta Branch](https://github.com/dakanji/G2Project/tree/beta)** - This branch contains the latest "Release Candidate" code and is always deployable. The current file set can be [DOWNLOADED HERE](https://github.com/dakanji/G2Project/archive/beta.zip).
- **[Alpha Branch](https://github.com/dakanji/G2Project/tree/alpha)** - This branch contains the latest semi-stable code and is normally deployable. This branch is never formally "issued" but the current file set can be [DOWNLOADED HERE](https://github.com/dakanji/G2Project/archive/alpha.zip).
- **Legacy Branches** - These contain important old official releases. These are for reference only and never updated.
- **Other Branches** - These contain ongoing work streams which may, or may not, be deployable. Note also that these may be deleted without notice.

# Updating Gallery 2

## From Version 2.2.6 and above

1.  Backup your installation including your database
2.  Update PHP to PHP 5.3, or, preferably, PHP 7.3
3.  Overwrite your installation with the [LATEST COMMUNITY VERSION](https://github.com/dakanji/G2Project/releases), navigate to your gallery and follow the on-screen instructions

## From Before Version 2.2.6

1.  Backup your installation including your database
2.  Update or downgrade PHP to versions between PHP 4.3 and PHP 5.2
3.  Overwrite your installation with a [Gallery v2.2.6 file set](https://github.com/dakanji/G2Project/archive/legacy/v2.2.6.zip), navigate to your gallery and follow the on-screen instructions
4.  When you are satisfied that the upgrade to Gallery v2.2.6 is complete, backup your Gallery v2.2.6 installation and database
5.  Update to PHP 5.3, or, preferably, PHP 7.3
6.  Overwrite your Gallery v2.2.6 installation with the [LATEST COMMUNITY VERSION](https://github.com/dakanji/G2Project/releases), navigate to your gallery and follow the on-screen instructions

# Upgrading From Gallery v1

[CLICK HERE](http://codex.galleryproject.org/Gallery2:migration) for instructions

# Upgrading From Gallery v3

It is currently not possible to auto upgrade from Gallery v3 to Gallery 2. You will need to manually transfer your Gallery v3 items.

# Contributing

## Code Enhancements and Fixes

Code enhancements and fixes are very much welcome. Please refer to the [Contribution Guidance](https://github.com/dakanji/G2Project/blob/alpha/CONTRIBUTING.md) for information on how to leverage the project developer environment.

## Issues / Bugs

Check the [Known Issues](http://codex.gallery2.org/Gallery2:Known_Issues) list and [Bug Tracker](https://github.com/dakanji/G2Project/issues) for information and some workarounds for known problems.
