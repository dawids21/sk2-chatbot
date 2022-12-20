import AuthContextProvider from "./context/AuthContextProvider";
import SnackbarContextProvider from "./context/SnackbarContextProvider";
import AppRoutes from "./components/routes/AppRoutes"
import AppBar from "./components/ui/appBar/AppBar";
import { ThemeProvider } from "@mui/material";
import theme from "./config/theme"
import SnackbarContext from "./context/snackbar-context";

const App = () => {
  return (
    <SnackbarContextProvider>
      <AuthContextProvider>
        <ThemeProvider theme={theme}>
          <header>
            <AppBar />
          </header>
          <main>
            <AppRoutes />
          </main>
        </ThemeProvider>
      </AuthContextProvider>
    </SnackbarContextProvider>
  );
};

export default App;
