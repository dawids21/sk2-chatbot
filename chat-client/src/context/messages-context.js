import React from "react";

const MessagesContext = React.createContext({
  messages: [],
  unreadMessages: [],
  getMessages: async (id) => {},
  isLoadingMessages: false,
  readMessage: (id) => {},
  sendMessage: (message) => {},
});

export default MessagesContext;
