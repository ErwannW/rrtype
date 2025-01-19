#include <gtest/gtest.h>
#include "Abn.hpp"

using namespace ABN;

TEST(Basic, Connection)
{
    Server server(1024, 1025);
    Client client(1024, 1025, 5000);

    ASSERT_TRUE(server.Open());
    ASSERT_TRUE(client.Connect());
    ASSERT_TRUE(client.Disconnect());
    ASSERT_TRUE(server.Close());
}

TEST(Basic, TCPMessageToServer)
{
    Server server(1026, 1027);
    Client client(1026, 1027, 5001);

    ASSERT_TRUE(server.Open());
    ASSERT_TRUE(client.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(client.Send(message, strlen(message), 42, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    server.GetPackets(packets);

    ASSERT_GT(packets.size(), 0);

    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    ASSERT_TRUE(client.Disconnect());
    ASSERT_TRUE(server.Close());
}

TEST(Basic, UDPMessageToServer)
{
    Server server(1028, 1029);
    Client client(1028, 1029, 5002);

    ASSERT_TRUE(server.Open());
    ASSERT_TRUE(client.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(client.Send(message, strlen(message), 42, false));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    server.GetPackets(packets);

    ASSERT_GT(packets.size(), 0);

    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    ASSERT_TRUE(client.Disconnect());
    ASSERT_TRUE(server.Close());
}

TEST(Basic, TCPMessageToClient)
{
    Server server(1030, 1031);
    Client client(1030, 1031, 5003);

    ASSERT_TRUE(server.Open());
    ASSERT_TRUE(client.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(server.Send(message, strlen(message), 42, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    client.GetPackets(packets);

    ASSERT_GT(packets.size(), 0);

    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    ASSERT_TRUE(client.Disconnect());
    ASSERT_TRUE(server.Close());
}

TEST(Basic, UDPMessageToClient)
{
    Server server(1032, 1033);
    Client client(1032, 1033, 5004);

    ASSERT_TRUE(server.Open());
    ASSERT_TRUE(client.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(server.Send(message, strlen(message), 42, false));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    client.GetPackets(packets);

    ASSERT_GT(packets.size(), 0);

    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    ASSERT_TRUE(client.Disconnect());
    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleConnection)
{
    Server server(1034, 1035);
    Client c1(1034, 1035, 5005);
    Client c2(1034, 1035, 5006);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleTCPMessageToServer)
{
    Server server(1036, 1037);
    Client c1(1036, 1037, 5007);
    Client c2(1036, 1037, 5008);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(c1.Send(message, strlen(message), 42, true));
    ASSERT_TRUE(c2.Send(message, strlen(message), 42, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    server.GetPackets(packets);

    ASSERT_GT(packets.size(), 1);
    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));
    char message3[18] = {0};

    memcpy(message3, packets.end()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message3, message));

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleUDPMessageToServer)
{
    Server server(1038, 1039);
    Client c1(1038, 1039, 5009);
    Client c2(1038, 1039, 5010);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(c1.Send(message, strlen(message), 42, false));
    ASSERT_TRUE(c2.Send(message, strlen(message), 42, false));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    server.GetPackets(packets);

    ASSERT_GT(packets.size(), 1);
    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));
    char message3[18] = {0};

    memcpy(message3, packets.end()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message3, message));

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleTCPMessageToClient)
{
    Server server(1040, 1041);
    Client c1(1040, 1041, 5011);
    Client c2(1040, 1041, 5012);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(server.Send(message, strlen(message), 42, true, c2.GetClientId()));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    c2.GetPackets(packets);

    ASSERT_GT(packets.size(), 0);
    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleUDPMessageToClient)
{
    Server server(1042, 1043);
    Client c1(1042, 1043, 5013);
    Client c2(1042, 1043, 5014);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(server.Send(message, strlen(message), 42, false, c2.GetClientId()));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::vector<packet_t> packets;
    c2.GetPackets(packets);

    ASSERT_GT(packets.size(), 0);
    char message2[18] = {0};
    memcpy(message2, packets.begin()->body_.data(), sizeof(packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleTCPMessageToClients)
{
    Server server(1044, 1045);
    Client c1(1044, 1045, 5015);
    Client c2(1044, 1045, 5016);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(server.Send(message, strlen(message), 42, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    std::vector<packet_t> c1_packets;
    c1.GetPackets(c1_packets);

    ASSERT_GT(c1_packets.size(), 0);
    char message2[18] = {0};
    memcpy(message2, c1_packets.begin()->body_.data(), sizeof(c1_packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    std::vector<packet_t> c2_packets;
    c2.GetPackets(c2_packets);

    ASSERT_GT(c2_packets.size(), 0);
    char message3[18] = {0};
    memcpy(message3, c2_packets.begin()->body_.data(), sizeof(c2_packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message3, message));

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}

TEST(Advenced, MultipleUDPMessageToClients)
{
    Server server(1046, 1047);
    Client c1(1046, 1047, 5017);
    Client c2(1046, 1047, 5018);

    ASSERT_TRUE(server.Open());

    ASSERT_TRUE(c1.Connect());
    ASSERT_TRUE(c2.Connect());

    constexpr char message[18] = "This is a message";
    ASSERT_TRUE(server.Send(message, strlen(message), 42, false));
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    std::vector<packet_t> c1_packets;
    c1.GetPackets(c1_packets);

    ASSERT_GT(c1_packets.size(), 0);
    char message2[18] = {0};
    memcpy(message2, c1_packets.begin()->body_.data(), sizeof(c1_packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message2, message));

    std::vector<packet_t> c2_packets;
    c2.GetPackets(c2_packets);

    ASSERT_GT(c2_packets.size(), 0);
    char message3[18] = {0};
    memcpy(message3, c2_packets.begin()->body_.data(), sizeof(c2_packets.begin()->header_.body_size_));
    ASSERT_TRUE(std::strcmp(message3, message));

    ASSERT_TRUE(c1.Disconnect());
    ASSERT_TRUE(c2.Disconnect());

    ASSERT_TRUE(server.Close());
}
