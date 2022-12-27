import React from "react";

const MessagesContext = React.createContext({
  messages: [],
  unreadMessages: [],
  getMessages: async (id) => {},
  isLoadingMessages: false,
});

export default MessagesContext;
