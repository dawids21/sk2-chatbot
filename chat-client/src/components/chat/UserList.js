import { Paper, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";

const UserList = ({ users, unreadMessages, current }) => {
  const navigate = useNavigate();
  if (users.length === 0) {
    return;
  }
  return (
    <>
      {users.map((user) => (
        <Paper
          sx={{
            mt: 1,
            p: 2,
            display: "flex",
            justifyContent: "space-between",
            cursor: "pointer",
            backgroundColor: unreadMessages.includes(user.user_id)
              ? "primary.light"
              : null,
          }}
          elevation={4}
          key={user.user_id}
          onClick={() => navigate(`/chat/${user.user_id}`)}
        >
          <Typography variant="h6">
            {current === user.user_id ? <u>{user.username}</u> : user.username}
          </Typography>
        </Paper>
      ))}
    </>
  );
};

export default UserList;
