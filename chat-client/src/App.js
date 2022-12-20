import AuthContextProvider from "./context/AuthContextProvider";
import SnackbarContextProvider from "./context/SnackbarContextProvider";

const App = () => {
  return (
    <AuthContextProvider>
      <SnackbarContextProvider>
      <h1>Hello, World!</h1>
      </SnackbarContextProvider>
    </AuthContextProvider>
  );
}

export default App;
