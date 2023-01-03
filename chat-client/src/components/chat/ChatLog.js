import { useContext, useEffect, useRef } from "react";
import AuthContext from "../../context/auth-context";
import MessagesContext from "../../context/messages-context";
import CenterCircularProgress from "../ui/CenterCircularProgress";
import Message from "./Message";

const ChatLog = ({ userId, username }) => {
  const { username: myUsername } = useContext(AuthContext);
  const messageRef = useRef(null);
  const { messages, isLoadingMessages, getMessages } =
    useContext(MessagesContext);

  useEffect(() => {
    getMessages(userId);
  }, [userId, getMessages]);

  const getMessageElements = () => {
    let result = [];
    for (let index = 0; index < messages.length; index++) {
      const message = messages[index];
      let messageElement;
      if (index === messages.length - 1) {
        messageElement = (
          <Message
            key={message.id}
            name={message.from === userId ? username : myUsername}
            timestamp={new Date(message.timestamp)}
            message={message.message}
            ref={messageRef}
          />
        );
      } else {
        messageElement = (
          <Message
            key={message.id}
            name={message.from === userId ? username : myUsername}
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
  }, [isLoadingMessages, messages]);

  return (
    <>
      {isLoadingMessages ? (
        <CenterCircularProgress />
      ) : (
        <>{getMessageElements()}</>
      )}
    </>
  );
};

export default ChatLog;
