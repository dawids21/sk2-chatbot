import { Button, Paper, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";

const UserList = ({
  users,
  friends,
  addFriendHandler,
  removeFriendHandler,
}) => {
  const navigate = useNavigate();
  if (users.length === 0) {
    return;
  }
  const friendIds = friends.map((friend) => friend.user_id);
  const addButton = (user) => {
    return (
      <Button
        variant="outlined"
        onClick={(event) =>
          addFriendHandler(event, user.user_id, user.username)
        }
      >
        Add
      </Button>
    );
  };
  const removeButton = (user) => {
    return (
      <Button
        variant="outlined"
        color="error"
        onClick={(event) => removeFriendHandler(event, user.user_id)}
      >
        Remove
      </Button>
    );
  };

  const handleClick = (userId) => {
    if (!friendIds.includes(userId)) {
      return;
    }
    navigate(`/chat/${userId}`);
  };
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
          }}
          elevation={4}
          key={user.user_id}
          onClick={() => handleClick(user.user_id)}
        >
          <Typography variant="h5">{user.username}</Typography>
          {friendIds.includes(user.user_id)
            ? removeButton(user)
            : addButton(user)}
        </Paper>
      ))}
    </>
  );
};

export default UserList;
