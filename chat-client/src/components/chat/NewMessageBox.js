import TextField from "@mui/material/TextField";
import { useState } from "react";

const NewMessageBox = ({ onSendMessage }) => {
  const [message, setMessage] = useState("");
  const changeHandler = (event) => {
    setMessage(event.target.value);
  };
  const sendHandler = (event) => {
    if (event.key === "Enter") {
      onSendMessage(message);
      setMessage("");
    }
  };
  return (
    <TextField
      margin="normal"
      id="message"
      type="text"
      fullWidth
      variant="outlined"
      value={message}
      onChange={changeHandler}
      onKeyUp={sendHandler}
    />
  );
};

export default NewMessageBox;
