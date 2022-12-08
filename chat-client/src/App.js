import { useState } from "react";

function App() {
  const [response, setResponse] = useState();

  const sendRequest = async () => {
    const res = await fetch("http://127.0.0.1:1100");
    console.log(res.statusText);
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
      <input id="url"></input>
      <br />
      <label htmlFor="data">Data: </label>
      <input id="data"></input>
      <br />
      <button onClick={sendRequest}>Send request</button>
      {response ? <p>{response}</p> : null}
    </>
  );
}

export default App;
