fetch("http://127.0.0.1:1100/sdfsd?waae", {
	method: 'POST',
	headers: {
		"Authorization": "12345"
	},
	body: JSON.stringify({ data: 'data' })
})
	.then((response) => response.json())
	.then((data) => console.log(data));