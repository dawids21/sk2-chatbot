import { useContext, useEffect, useRef, useState } from "react";
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
  const messageRef = useRef(null);
  useEffect(() => {
    const getUser = async () => {
      setIsUsernameLoading(true);
      const response = await fetch(`${backend.url}/info`, {
        method: "POST",
        body: JSON.stringify({ id: userId }),
        headers: {
          Authorization: token,
        },
      });
      if (response.status !== 200) {
        setUsername("User");
        setIsUsernameLoading(false);
        return;
      }
      const data = await response.json();
      setUsername(data.username);
      setIsUsernameLoading(false);
    };
    getUser();
  }, [userId, token]);

  useEffect(() => {
    const setInfo = async () => {
      setIsMyUsernameLoading(true);
      const info = await getInfo();
      setMyUsername(info.username);
      setIsMyUsernameLoading(false);
    };
    setInfo();
  }, [getInfo]);

  useEffect(() => {
    const getMessages = async () => {
      setAreMessagesLoading(true);
      const response = await fetch(`${backend.url}/messages`, {
        method: "POST",
        body: JSON.stringify({ friend_id: userId }),
        headers: {
          Authorization: token,
        },
      });
      if (response.status !== 200) {
        alert("Can't get messages", "error");
        setMessages([]);
        setAreMessagesLoading(false);
        return;
      }
      const data = await response.json();
      setMessages(data);
      setAreMessagesLoading(false);
    };
    getMessages();
  }, [userId, token, alert]);

  const isLoading =
    isUsernameLoading || isMyUsernameLoading || areMessagesLoading;

  const getMessages = () => {
    let result = [];
    for (let index = 0; index < messages.length; index++) {
      const message = messages[index];
      let messageElement;
      if (index === messages.length - 1) {
        messageElement = (
          <Message
            key={message.id}
            name={message.user_id === parseInt(userId) ? username : myUsername}
            timestamp={new Date(message.timestamp)}
            message={message.message}
            ref={messageRef}
          />
        );
      } else {
        messageElement = (
          <Message
            key={message.id}
            name={message.user_id === parseInt(userId) ? username : myUsername}
            timestamp={new Date(message.timestamp)}
            message={message.message}
          />
        );
      }
      result.push(messageElement);
    }
    return result;
  };

  useEffect(() => {
    messageRef.current?.scrollIntoView();
  }, [isLoading]);

  return <>{isLoading ? <CenterCircularProgress /> : <>{getMessages()}</>}</>;
};

export default ChatLog;
