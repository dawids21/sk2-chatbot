import { useParams } from "react-router-dom";

const Chat = () => {
    const { userId } = useParams();
    return <h1>Chat with {userId}</h1>;
};

export default Chat;