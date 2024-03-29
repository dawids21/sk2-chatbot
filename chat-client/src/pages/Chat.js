import { Box } from "@mui/material";
import Grid2 from "@mui/material/Unstable_Grid2";
import { useContext, useEffect, useMemo, useState } from "react";
import { useParams } from "react-router-dom";
import ChatLog from "../components/chat/ChatLog";
import NewMessageBox from "../components/chat/NewMessageBox";
import UserList from "../components/chat/UserList";
import CenterCircularProgress from "../components/ui/CenterCircularProgress";
import backend from "../config/backend";
import AuthContext from "../context/auth-context";
import MessagesContext from "../context/messages-context";
import useSnackbar from "../hooks/use-snackbar";

const Chat = () => {
  const { userId: userIdStr } = useParams();
  const userId = useMemo(
    () => (userIdStr !== undefined ? parseInt(userIdStr) : null),
    [userIdStr]
  );
  const [friends, setFriends] = useState([]);
  const [isLoading, setIsLoading] = useState(true);
  const { isLoggedIn, token } = useContext(AuthContext);
  const { unreadMessages, readMessage, sendMessage } =
    useContext(MessagesContext);
  const alert = useSnackbar();
  useEffect(() => {
    const getFriends = async () => {
      setIsLoading(true);
      const response = await fetch(`${backend.url}/friends`, {
        method: "GET",
        headers: {
          Authorization: token,
        },
      });
      if (response.status !== 200) {
        alert("Something went wrong!", "error");
        return;
      }
      const data = await response.json();
      setFriends(data);
      setIsLoading(false);
    };
    getFriends();
  }, [alert, isLoggedIn, token]);

  useEffect(() => {
    readMessage(userId);
  }, [userId, readMessage]);

  const sendMessageHandler = (message) => {
    sendMessage(message);
  };

  const getFriendUsername = () => {
    const friend = friends.find((friend) => friend.user_id === userId);
    if (!friend) {
      return "";
    }
    return friend.username;
  };
  return (
    <Grid2 container spacing={2} sx={{ m: 2, mb: 0 }}>
      <Grid2 sm={4} md={3} lg={2}>
        {isLoading ? (
          <CenterCircularProgress />
        ) : (
          <UserList
            users={friends}
            unreadMessages={unreadMessages}
            current={userId}
          />
        )}
      </Grid2>
      <Grid2 sm={8} md={9} lg={10}>
        {userId ? (
          <Box
            sx={{
              overflow: "hidden",
              maxHeight: "calc(100vh - 96px)",
              height: "calc(100vh - 96px)",
              display: "flex",
              flexDirection: "column",
            }}
          >
            <Box sx={{ overflowY: "auto", flexGrow: 1 }}>
              <ChatLog userId={userId} username={getFriendUsername()} />
            </Box>
            <NewMessageBox onSendMessage={sendMessageHandler} />
          </Box>
        ) : null}
      </Grid2>
    </Grid2>
  );
};

export default Chat;
