import React from "react";

const SnackbarContext = React.createContext({
  alert: (message, severity) => {},
});

export default SnackbarContext;
