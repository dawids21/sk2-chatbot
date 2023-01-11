import AuthContextProvider from "./context/AuthContextProvider";
import SnackbarContextProvider from "./context/SnackbarContextProvider";
import AppRoutes from "./components/routes/AppRoutes";
import AppBar from "./components/ui/appBar/AppBar";
import { ThemeProvider } from "@mui/material";
import theme from "./config/theme";
import MessagesContextProvider from "./context/MessagesContextProvider";

const App = () => {
  return (
    <SnackbarContextProvider>
      <AuthContextProvider>
        <MessagesContextProvider>
          <ThemeProvider theme={theme}>
            <header>
              <AppBar />
            </header>
            <main>
              <AppRoutes />
            </main>
          </ThemeProvider>
        </MessagesContextProvider>
      </AuthContextProvider>
    </SnackbarContextProvider>
  );
};

export default App;
