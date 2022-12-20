import { useContext } from "react";
import SnackbarContext from "../context/snackbar-context";

const useSnackbar = () => {
  const snackbarCtx = useContext(SnackbarContext);
  return snackbarCtx.alert;
};

export default useSnackbar;
