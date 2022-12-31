import { useContext, useEffect, useRef, useState } from "react";
import backend from "../../config/backend";
import AuthContext from "../../context/auth-context";
import MessagesContext from "../../context/messages-context";
import CenterCircularProgress from "../ui/CenterCircularProgress";
import Message from "./Message";

const ChatLog = ({ userId }) => {
  const [username, setUsername] = useState("");
  const [isUsernameLoading, setIsUsernameLoading] = useState(true);
  const { token, username: myUsername } = useContext(AuthContext);
  const messageRef = useRef(null);
  const { messages, isLoadingMessages, getMessages } =
    useContext(MessagesContext);
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
    getMessages(userId);
  }, [userId, getMessages]);

  const isLoading = isUsernameLoading || isLoadingMessages;

  const getMessageElements = () => {
    let result = [];
    for (let index = 0; index < messages.length; index++) {
      const message = messages[index];
      let messageElement;
      if (index === messages.length - 1) {
        messageElement = (
          <Message
            key={message.timestamp}
            name={message.user_id === userId ? username : myUsername}
            timestamp={new Date(message.timestamp)}
            message={message.message}
            ref={messageRef}
          />
        );
      } else {
        messageElement = (
          <Message
            key={message.timestamp}
            name={message.user_id === userId ? username : myUsername}
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
  }, [isLoading, messages]);

  return (
    <>{isLoading ? <CenterCircularProgress /> : <>{getMessageElements()}</>}</>
  );
};

export default ChatLog;
