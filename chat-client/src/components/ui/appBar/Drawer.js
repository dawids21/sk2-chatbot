import React from "react";
import MUIDrawer from "@mui/material/Drawer";
import {
  Box,
  Divider,
  List,
  ListItem,
  ListItemButton,
  ListItemText,
  Typography,
} from "@mui/material";

const Drawer = (props) => {
  const { toggle, open, items } = props;
  const content = (
    <Box onClick={toggle} sx={{ textAlign: "center" }}>
      <Typography variant="h5" component="h1" sx={{ my: 2 }}>
        Chat
      </Typography>
      <Divider />
      <List>
        {items.map((item) => (
          <ListItem key={item.name} disablePadding>
            <ListItemButton onClick={item.action} sx={{ textAlign: "center" }}>
              <ListItemText primary={item.name} />
            </ListItemButton>
          </ListItem>
        ))}
      </List>
    </Box>
  );

  return (
    <MUIDrawer
      open={open}
      onClose={toggle}
      ModalProps={{
        keepMounted: true
      }}
      sx={{
        display: { xs: "block", sm: "none" },
        "& .MuiDrawer-paper": {
          boxSizing: "border-box",
          width: 240,
        },
      }}
    >
      {content}
    </MUIDrawer>
  );
};

export default Drawer;
