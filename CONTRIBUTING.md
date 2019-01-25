# CONTRIBUTING

Thank you for your interest in contributing to G2Project which seeks to add enhancements and fixes to the Gallery 2 code so that its several loyal fans can continue using this awesome piece of software engineering

The project has been set up with resources to ensure that your contributed code meets the coding style requirements with a minimum of effort on your part.

## Prerequisites

You will need to have Git and Composer installed on your development machine.

- [Git](https://git-scm.com/)
- [Composer](https://getcomposer.org/)

## Setup

If you wish to contribute to this project, fork the project here on Github and on your computer, clone the fork using your favourite Git agent or manually:

```bash
git clone https://github.com/YOUR_GITHUB_USERNAME/G2Project-PREFIX G2Project-PREFIX
```


Then open the folder in your command line, and install the needed dependencies with a single commend:

```bash
composer update
```

This will install various composer libraries into a "dev_vendor" folder.
You can start coding and every commit you make will be automatically checked for php errors and also amemded to match the code style as required using a Git pre-commit hook.

If you use Sourcetree as your Git Client, note that it installs with the option to use the embedded git version enabled by default as of Sourcetree v2.7.6, this git version is Git v2.8.x which DOES NOT include the pre-commit hook facility.

Modern Macs ship with at least Git v2.9.x which includes this facility. Please go to **Preferences -> Git** and select "Use System Git" to activate the project pre-commit hook. 

In Sourcetree for Windows, please go to **Tools -> Options -> Git** select "Use System Git" if you have Git v2.9.x or above installed. 
There is also a function to update the version of Git which is embedded in the installation of Sourcetree for Windows.
 
Thank you once again
