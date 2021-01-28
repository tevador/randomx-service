# RandomX Service

RandomX Service provides a simple HTTP API to calculate [RandomX](https://github.com/tevador/RandomX) proof of work (PoW).

## Use cases

* Fast PoW verification for Node.js based pools
* Web-mining with explicit user consent (see below)
* Access to RandomX for clients who cannot link directly to librandomx

## Building

### Linux
Prerequisites: cmake, make, gcc
```
git clone --recursive https://github.com/tevador/randomx-service
cd randomx-service
./build.sh
```

### Windows
Prerequisites: cmake, Visual Studio
```
git clone --recursive https://github.com/tevador/randomx-service
cd randomx-service
build.cmd
```

## Usage

```
Usage: ./randomx-service [OPTIONS]
Supported options:
  -host <string>     Bind to a specific address (default: localhost)
  -port <number>     Bind to a specific port (default: 39093)
  -threads <number>  Use a specific number of threads (default: all CPU threads)
  -flags <number>    Use specific RandomX flags (default: auto)
  -origin <string>   Allow cross-origin requests from a specific web page
  -log               Log all HTTP requests to stdout
  -help              Display this message
```

## RandomX Service API

The service provides the following API methods:

* `GET /info`
* `POST /seed`
* `POST /hash`
* `POST /batch`

Refer to [doc/API.md](doc/API.md).

## Web-mining

To allow requests from the browser, use the `-origin` command line option. For example, to allow the site https://example.com to access the service, this command line option should be used:

```
./randomx-service -origin https://example.com
```

### Notes
* using the `/batch` API, web-miners are able to reach similar performance as native miners
* HTTP requests to localhost from HTTPS websites are currently blocked by [Safari](https://bugs.webkit.org/show_bug.cgi?id=171934), but this behavior may change in the future (refer to the linked bugtracker).

## Donations

XMR:
```
845xHUh5GvfHwc2R8DVJCE7BT2sd4YEcmjG8GNSdmeNsP5DTEjXd1CNgxTcjHjiFuthRHAoVEJjM7GyKzQKLJtbd56xbh7V
```
