# Summary of changes

## Changes for version 2.0.2 (15 Feb 2022)

### Bug Fixes

- Cleansed certificate strings for dump protection


## Changes for version 2.0.1 (8 Feb 2022)

### Bug Fixes

- Moved to version 2.0.1 of WebServerAdapter:
  - Fixed security configuration object default's constructor

## Changes for version 2.0.0 (23 Dec 2021)

### Breaking changes

- Moved to version 2.0.0 (new interface) of WebServerAdapter


## Changes for version 1.3.7 (30 Sep 2021)

### Bug Fixes

- Moved continuous integration to Jenkins
- Deploy conan package into remote specific for C++ task force
- Moved WebServerAdapterInterface dependency to version 1.1.12
- Added configurations for the Snow project into continuous integration
- Fixed use of "export_sources" definition in conan recipes


## Changes for version 1.3.6 (1 Apr 2021)

### Bug Fixes

- Moved deploy of Conan packages to CSW JFrogPlatform


## Changes for version 1.3.5 (25 Feb 2021)

### Bug Fixes

- Updated continuous integration to:
  - Use GitHub Action to generate library documentation
  - Upload generated packages to Systelab's public Artifactory


## Changes for version 1.3.4 (4 Sep 2020)

### Bug Fixes

- Moved WebServerAdapterInterface dependency to version 1.1.7


## Changes for version 1.3.3 (4 Sep 2020)

### Bug Fixes

- Moved WebServerAdapterInterface dependency to version 1.1.6 (which includes "uriFull" attribute on requests)
- Fixed parsing of query strings


## Changes for version 1.3.2 (2 Jun 2020)

### Bug Fixes

- Used gtest from conan-center (without username/channel)


## Changes for version 1.3.1 (26 May 2020)

### Bug Fixes

- Added example to show how to set up a secured web server


## Changes for version 1.3.0 (22 May 2020)

### Enhancements

- Added support for OpenSSL 1.1.1
- Allow disabling specific TLS versions on secured servers


## Changes for version 1.2.3 (21 May 2020)

### Bug Fixes

- Moved WebServerAdapterInterface dependency to version 1.1.3


## Changes for version 1.2.2 (7 Feb 2020)

### Bug Fixes

- Fixed compilation for GoogleTest version 1.10.0
- Fixed compilation for Visual Studio 2019


## Changes for version 1.2.1 (15 Jan 2020)

### Bug Fixes

- Fixed issue on Conan package


## Changes for version 1.2.0 (15 Jan 2020)

### Enhancements

- Added implementation for HTTP/HTTPS client interface


## Changes for version 1.1.1 (14 Jan 2020)

### Bug Fixes

- Fixed issue on SSL usage with DH param


## Changes for version 1.1.0 (14 Jan 2020)

### Enhancements

- Added SSL support


## Changes for version 1.0.1 (13 Jan 2020)

### Bug Fixes

- Considered configuration option for GZIP compression


## Changes for version 1.0.0 (12 Jan 2020)

### Enhancements

- Initial version in GitHub

