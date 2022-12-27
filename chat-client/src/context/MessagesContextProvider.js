import { useCallback, useContext, useEffect, useRef, useState } from "react";
import backend from "../config/backend";
import useSnackbar from "../hooks/use-snackbar";
import AuthContext from "./auth-context";
import MessagesContext from "./messages-context";

const MessagesContextProvider = (props) => {
  const [messages, setMessages] = useState([]);
  const [unreadMessages, setUnreadMessages] = useState([]);
  const [isLoadingMessages, setIsLoadingMessages] = useState(false);
  const [userId, setUserId] = useState(null);
  const alert = useSnackbar();
  const { token, isLoggedIn } = useContext(AuthContext);
  const ws = useRef(null);

  const getMessagesHandler = useCallback(
    async (id) => {
      setIsLoadingMessages(true);
      const response = await fetch(`${backend.url}/messages`, {
        method: "POST",
        body: JSON.stringify({ friend_id: id }),
        headers: {
          Authorization: token,
        },
      });
      if (response.status !== 200) {
        alert("Can't get messages", "error");
        setMessages([]);
        setIsLoadingMessages(false);
        return;
      }
      const data = await response.json();
      setUserId(id);
      setMessages(data);
      setIsLoadingMessages(false);
    },
    [alert, token]
  );

  useEffect(() => {
    let socket;
    if (isLoggedIn && token) {
      socket = new WebSocket("ws://localhost:5000");

      socket.onopen = () => {
        socket.send(
          JSON.stringify({
            operation: "/start",
            token: token,
          })
        );
      };

      socket.onclose = () => {
        console.log("closed");
      };

      socket.onmessage = (event) => {
        const data = JSON.parse(event.data);
        const from = parseInt(data.from);
        if (from === userId) {
          setMessages((prevMessages) => [
            ...prevMessages,
            {
              id: parseInt(data.id),
              user_id: parseInt(data.user_id),
              message: data.message,
              timestamp: data.timestamp,
            },
          ]);
        } else {
          setUnreadMessages((prevUnreadMessages) => {
            if (!prevUnreadMessages.includes(from)) {
              return [...prevUnreadMessages, from];
            }
            return prevUnreadMessages;
          });
        }
      };

      ws.current = socket;
    }

    return () => {
      if (socket) {
        socket.close();
      }
    };
  }, [isLoggedIn, token, userId]);

  const readMessage = (id) => {
    setUnreadMessages((prevUnreadMessages) => {
      if (prevUnreadMessages.includes(id)) {
        return prevUnreadMessages.filter((userId) => userId !== id);
      }
      return prevUnreadMessages;
    });
  };

  return (
    <MessagesContext.Provider
      value={{
        messages,
        unreadMessages,
        isLoadingMessages,
        getMessages: getMessagesHandler,
        readMessage: readMessage,
      }}
    >
      {props.children}
    </MessagesContext.Provider>
  );
};

export default MessagesContextProvider;
