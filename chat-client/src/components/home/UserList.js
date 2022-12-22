import { Button, Paper, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";

const UserList = ({ users, friends, addFriendHandler, removeFriendHandler }) => {
  const navigate = useNavigate();
  if (users.length === 0) {
    return;
  }
  const friendIds = friends.map((friend) => friend.id);
  const addButton = (user) => {
    return (
      <Button variant="outlined" onClick={() => addFriendHandler(user.id, user.username)}>
        Add
      </Button>
    );
  };
  const removeButton = (user) => {
    return (
      <Button variant="outlined" color="error" onClick={() => removeFriendHandler(user.id)}>
        Remove
      </Button>
    );
  };
  return (
    <>
      {users.map((user) => (
        <Paper
          sx={{ mt: 1, p: 2, display: "flex", justifyContent: "space-between", cursor: "pointer" }}
          elevation={4} key={user.id} onClick={() => navigate(`/chat/${user.id}`)}
        >
          <Typography variant="h5">
            {user.username}
          </Typography>
          {friendIds.includes(user.id) ? removeButton(user) : addButton(user)}
        </Paper>
      ))}
    </>
  );
};

export default UserList;
