const fetch = require('node-fetch');

function randomx_request(url, data, content) {
	return fetch(url, {
		method: 'POST',
		body: data,
		headers: {
			"Content-Type": content,
			"Accept": content
		}
	});
}

const hashes = 10000;
const hashingBlob = Buffer.from('4c0b0b98bea7e805e0010a2126d287a2a0cc833d312cb786385a7c2f9de69d25537f584a9bc9977b00000000666fd8753bf61a8631f12984e3fd44f4014eca629276817b56f32e9b68bd82f416', 'hex');
const batchSize = 256;

async function bechmark() {
	let start = Date.now();
	let nonce = 0;
	let offset = 0;
	for (let i = hashes; i > 0; i -= batchSize) {
		let batch = Buffer.alloc(0);
		let size = i < batchSize ? i : batchSize;
		console.log("batch: " + size);
		for (let j = 0; j < size; ++j) {
			hashingBlob.writeUInt32LE(nonce++, 40);
			batch = Buffer.concat([batch, hashingBlob]);
		}
		var res = await randomx_request('http://localhost:39093/batch', batch, 'application/x.randomx.batch+bin');
		if (res.status != 200)
			throw Error("Unexpected status: HTTP " + res.status);
	}
	let end = Date.now();
	console.log("Perf: " + 1000 * hashes / (end - start) + " H/s");
}

bechmark();
