## RandomX Service API

### GET /info

Returns information about the service instance, in JSON format:
* service version
* the supported algorithm (always `rx/0`)
* the maximum number of parallel requests the service can support
* the current RandomX seed (in hex format)
* the total number of hashes the service has calculated

#### Example

```
curl http://localhost:39093/info
```
```json
{
	"randomx_service": "v1.0.1",
	"algorithm": "rx/0",
	"threads": 2,
	"seed": "74657374206b657920303030",
	"hashes": 1
}
```

### POST /seed

Reinitializes the RandomX cache and dataset with the provided seed value. The seed is extracted from the request body based on the `Content-Type` header.

This request is exclusive - it will block until all preceding requests have completed and all subsequent requests to the service will be paused until the reseeding process is complete. This ensures that all hashes are always calculated with a well-defined seed value.

#### Headers

##### Content-Type: `application/x.randomx+bin`
* the POST body is interpreted as a binary literal

##### Content-Type: `application/x.randomx+hex`
* the POST body is interpreted as a base16 (hex) encoded value

#### Responses
##### 204 No Content
* the request was successful; all subsequent hashes will be calculated with the new seed

##### 400 Bad Request
* the request body is malformed

##### 413 Payload Too Large
* the provided seed value is larger than 60 bytes

##### 415 Unsupported Media Type
* the `Content-Type` header is missing or has an unsupported value

#### Example

```
curl -X POST http://localhost:39093/seed -H "Content-Type: application/x.randomx+bin" -d "test key 000"
```

### POST /hash

Calculates a RandomX hash value of the provided input. The input is extracted from the request body based on the `Content-Type` header.

#### Headers

##### `Content-Type: application/x.randomx+bin`
* the POST body is interpreted as a binary literal

##### `Content-Type: application/x.randomx+hex`
* the POST body is interpreted as a base16 (hex) encoded value

##### `Accept: application/x.randomx+bin`
* the hash will be provided in binary format
* this header is optional

##### `RandomX-Seed: [base16]`
* sets the RandomX seed required for this hash to be valid
* the seed value must be in base16 (hex) format
* this header is optional
* it is recommended to provide this header to avoid invalid hashes caused by reseeding

#### Responses
##### 200 OK
* the request was successful; the response body contains the hash value, encoded as a base16 (hex) string (64 characters)
* if the `Accept` header was set to `application/x.randomx+bin`, the hash is provided as a binary value (32 bytes)

##### 400 Bad Request
* the request body is malformed

##### 403 Forbidden
* the RandomX cache and dataset have not been initialized (the `/seed` method must be called first)

##### 413 Payload Too Large
* the POST body is larger than 20000 bytes

##### 415 Unsupported Media Type
* the `Content-Type` header is missing or has an unsupported value

##### 422 Unprocessable Entity
* the `RandomX-Seed` header was provided and it doesn't match the current seed value

#### Example

```
curl -X POST http://localhost:39093/hash -H "Content-Type: application/x.randomx+bin" -d "This is a test"
```
```
639183aae1bf4c9a35884cb46b09cad9175f04efd7684e7262a0ac1c2f0b4e3f
```

### POST /batch

Calculates up to 256 RandomX hashes at once. The list of input values is provided in the request body and interpreted based on the `Content-Type` header.

#### Headers
##### `Content-Type: application/x.randomx.batch+bin`
* the POST body is interpreted as a binary sequence of length-prefixed inputs; the length of each input may not exceed 127 bytes (the length prefix is a single byte)

##### `Content-Type: application/x.randomx.batch+hex`
* the POST body is interpreted as a list of base16 (hex) encoded inputs separated by a single space character

##### `Accept: application/x.randomx.batch+bin`
* the hashes will be provided in binary format
* this header is optional

##### `RandomX-Seed: [base16]`
* sets the RandomX seed required for this batch of hashes to be valid
* the seed value must be in base16 (hex) format
* this header is optional
* it is recommended to provide this header to avoid invalid hashes caused by reseeding

#### Responses
##### 200 OK
* the request was successful; the response body contains the hashes of the requested inputs
* by default, the hashes are provided in hex format (64 characters) separated by a single space character
* if the `Accept` header was set to `application/x.randomx.batch+bin`, the hashes are provided in binary length-prefixed format (each hash is 32 bytes, so the prefix is `0x20`, which is conincidentally also a space character)

##### 400 Bad Request
* the request body is empty or malformed

##### 403 Forbidden
* the RandomX cache and dataset have not been initialized (the `/seed` method must be called first)

##### 413 Payload Too Large
* the POST body is larger than 20000 bytes or the batch contains more than 256 inputs

##### 415 Unsupported Media Type
* the `Content-Type` header is missing or has an unsupported value

##### 422 Unprocessable Entity
* the `RandomX-Seed` header was provided and it doesn't match the current seed value

#### Example

```
curl -X POST http://localhost:39093/batch -H "Content-Type: application/x.randomx.batch+hex" -d "74657374203031 74657374203032 74657374203033 74657374203034"
```
```
59fd4ca6eec3c2e60f67cd7605568c2da650b5e2beea5c563a7d0383b42e26b1 3a630fc27de8badc347aac4400fcfb261b1b0e0e75b393f50b1d5dc2603d5bef 600062e17f1b5aa6a907a94b9f787f465ab8ad142fb08261fc6ea12befa1bb97 aacdfc478af56ce1574db920ff48b88c0ab531b6090ffb44ac03bccb4f0d0fa8
```