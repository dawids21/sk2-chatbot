import { Alert, Snackbar } from "@mui/material";
import React, { useCallback, useEffect, useState } from "react";
import SnackbarContext from "./snackbar-context";

const SnackbarContextProvider = (props) => {
  const [snackbars, setSnackbars] = useState([]);
  const [open, setOpen] = useState(false);
  const [messageInfo, setMessageInfo] = useState(null);

  useEffect(() => {
    if (snackbars.length && !messageInfo) {
      setMessageInfo({ ...snackbars[0] });
      setSnackbars((prev) => prev.slice(1));
      setOpen(true);
    } else if (snackbars.length && messageInfo && open) {
      setOpen(false);
    }
  }, [snackbars, messageInfo, open]);

  const handleAlert = useCallback((alertMessage, severity) => {
    setSnackbars((prev) => [...prev, { message: alertMessage, severity }]);
  }, []);

  const handleClose = (event, reason) => {
    if (reason === "clickaway") {
      return;
    }
    setOpen(false);
  };

  const handleExited = () => {
    setMessageInfo(null);
  };

  return (
    <SnackbarContext.Provider
      value={{
        alert: handleAlert,
      }}
    >
      {props.children}
      <Snackbar
        open={open}
        autoHideDuration={3000}
        onClose={handleClose}
        TransitionProps={{ onExited: handleExited }}
      >
        <Alert
          severity={messageInfo?.severity}
          onClose={handleClose}
          variant="filled"
          sx={{ width: "100%" }}
        >
          {messageInfo?.message}
        </Alert>
      </Snackbar>
    </SnackbarContext.Provider>
  );
};

export default SnackbarContextProvider;
