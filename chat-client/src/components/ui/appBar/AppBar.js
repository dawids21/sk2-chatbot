import styled from "@emotion/styled";
import {
  AppBar as MUIAppBar,
  Box,
  Button,
  IconButton,
  Toolbar,
  useMediaQuery,
} from "@mui/material";
import MenuIcon from "@mui/icons-material/Menu";
import React, { useContext, useState } from "react";
import Drawer from "./Drawer";
import { useNavigate } from "react-router-dom";
import AuthContext from "../../../context/auth-context";
import Logo from "../Logo";

const Offset = styled("div")(({ theme }) => theme.mixins.toolbar);

const AppBar = () => {
  const [drawerOpen, setDrawerOpen] = useState(false);
  const isDrawerButtonVisible = useMediaQuery((theme) =>
    theme.breakpoints.down("sm")
  );
  const { isLoggedIn, onLogout } = useContext(AuthContext);
  const navigate = useNavigate();

  const handleDrawerToggle = () => {
    setDrawerOpen((prevDrawerOpen) => !prevDrawerOpen);
  };

  let buttons;
  if (!isLoggedIn) {
    buttons = [
      {
        name: "Login",
        action: () => navigate("/login"),
      },
    ];
  } else {
    buttons = [
      {
        name: "Logout",
        action: () => {
          navigate("/")
          onLogout();
        }
      }
    ];
  }

  return (
    <>
      <MUIAppBar position="fixed" component="nav">
        <Toolbar>
          {isDrawerButtonVisible && (
            <IconButton
              color="inherit"
              aria-label="open drawer"
              edge="start"
              onClick={handleDrawerToggle}
            >
              <MenuIcon />
            </IconButton>
          )}
          <Box
            sx={{
              display: "flex",
              alignItems: "center",
              alignSelf: "stretch",
              userSelect: "none",
              cursor: "pointer",
            }}
            onClick={() => navigate("/")}
          >
            <Logo variant="h4" />
          </Box>
          <Box sx={{ flexGrow: 1 }}></Box>
          {!isDrawerButtonVisible && (
            <Box>
              {buttons.map((button) => (
                <Button key={button.name} onClick={button.action} color="white">
                  {button.name}
                </Button>
              ))}
            </Box>
          )}
        </Toolbar>
      </MUIAppBar>
      <Box component="nav">
        <Drawer open={drawerOpen} toggle={handleDrawerToggle} items={buttons} />
      </Box>
      <Offset />
    </>
  );
};

export default AppBar;
