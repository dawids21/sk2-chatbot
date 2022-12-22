import { useContext, useEffect, useState } from "react";
import backend from "../../config/backend";
import AuthContext from "../../context/auth-context";
import useSnackbar from "../../hooks/use-snackbar";
import CenterCircularProgress from "../ui/CenterCircularProgress";
import Message from "./Message";

const ChatLog = ({ userId }) => {
  const [username, setUsername] = useState("");
  const [myUsername, setMyUsername] = useState("");
  const [messages, setMessages] = useState([]);
  const [isUsernameLoading, setIsUsernameLoading] = useState(true);
  const [isMyUsernameLoading, setIsMyUsernameLoading] = useState(true);
  const [areMessagesLoading, setAreMessagesLoading] = useState(true);
  const { token, getInfo } = useContext(AuthContext);
  const alert = useSnackbar();
  useEffect(() => {
    const getUser = async () => {
      setIsUsernameLoading(true);
      const response = await fetch(`${backend.url}/info`, {
        method: "POST",
        body: JSON.stringify({ id: userId }),
        headers: {
          Authorization: token
        }
      })
      if (response.status !== 200) {
        setUsername("User");
        setIsUsernameLoading(false);
        return;
      }
      const data = await response.json();
      setUsername(data.username);
      setIsUsernameLoading(false);
    }
    getUser();
  }, [userId, token]);

  useEffect(() => {
    const setInfo = async () => {
      setIsMyUsernameLoading(true);
      const info = await getInfo();
      setMyUsername(info.username);
      setIsMyUsernameLoading(false);
    }
    setInfo();
  }, [getInfo])

  useEffect(() => {
    const getMessages = async () => {
      setAreMessagesLoading(true);
      const response = await fetch(`${backend.url}/messages`, {
        method: "POST",
        body: JSON.stringify({ friend_id: userId }),
        headers: {
          Authorization: token
        }
      })
      if (response.status !== 200) {
        alert("Can't get messages", "error");
        setMessages([]);
        setAreMessagesLoading(false);
        return;
      }
      const data = await response.json();
      setMessages(data);
      setAreMessagesLoading(false);
    }
    getMessages();
  }, [userId, token, alert]);

  const isLoading = isUsernameLoading || isMyUsernameLoading || areMessagesLoading;

  return (
    <>
      {
        isLoading ? <CenterCircularProgress /> : (
          <>
            {messages.map((message) => (
              <Message
                key={message.timestamp}
                name={message.user_id === parseInt(userId) ? username : myUsername}
                timestamp={new Date(message.timestamp)}
                message={message.message}
              />
            ))}
          </>
        )
      }
    </>
  );
}

export default ChatLog;