import { useCallback, useContext, useState } from "react";
import backend from "../config/backend";
import useSnackbar from "../hooks/use-snackbar";
import AuthContext from "./auth-context";
import MessagesContext from "./messages-context";

const MessagesContextProvider = (props) => {
  const [messages, setMessages] = useState([]);
  const [unreadMessages, setUnreadMessages] = useState([]);
  const [isLoadingMessages, setIsLoadingMessages] = useState(false);
  const alert = useSnackbar();
  const { token } = useContext(AuthContext);

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
      setMessages(data);
      setIsLoadingMessages(false);
    },
    [alert, token]
  );

  return (
    <MessagesContext.Provider
      value={{
        messages,
        unreadMessages,
        isLoadingMessages,
        getMessages: getMessagesHandler,
      }}
    >
      {props.children}
    </MessagesContext.Provider>
  );
};

export default MessagesContextProvider;
