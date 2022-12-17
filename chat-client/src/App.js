import { useState } from "react";

function App() {
  const [response, setResponse] = useState();
  const [url, setUrl] = useState("");
  const [data, setData] = useState("");

  const handleChangeUrl = (event) => {
    setUrl(event.target.value);
  }

  const handleChangeData = (event) => {
    setData(event.target.value);
  }

  const sendRequest = async () => {
    console.log(url);
    let res;
    if (data === "") {
      res = await fetch("http://127.0.0.1:1100" + url);
    } else {
      let body = "";
      for (let index = 0; index < 2000; index++) {
        body = body + data;
      }
      res = await fetch("http://127.0.0.1:1100" + url, {
        method: "POST",
        body: JSON.stringify({ data: body }),
        headers: {
          Authorization: "12345"
        }
      });
    }
    try {
      const data = await res.json();
      setResponse(JSON.stringify(data))
    } catch (err) {
      setResponse("no body");
    }
  }
  return (
    <>
      <h1>Hello, World!</h1>
      <label htmlFor="url">URL: </label>
      <input id="url" onChange={handleChangeUrl} value={url}></input>
      <br />
      <label htmlFor="data">Data: </label>
      <input id="data" onChange={handleChangeData} value={data}></input>
      <br />
      <button onClick={sendRequest}>Send request</button>
      {response ? <p>{response}</p> : null}
    </>
  );
}

export default App;
