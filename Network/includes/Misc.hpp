/*
** EPITECH PROJECT, 2024
** Misc.cpp
** File description:
** Definition for Misc
*/

#ifndef MISC_HPP
#define MISC_HPP

#include <thread>
#include <cstdint>
#include <iostream>
#include <asio.hpp>

// Constants for IPv4 MTU size and headers
#define IPV4_MTU 1480 /**< Maximum Transmission Unit (MTU) for IPv4. */
#define IPV4_MTU_HEADER_LESS 1472 /**< MTU size minus the header size. */

// Constants for server configuration
#define SERVER_ID 1 /**< Unique identifier for the server. */

// Terminal color codes for logging and debugging output
#define RESET   "\033[0m" /**< Reset terminal color. */
#define BLACK   "\033[30m" /**< Set terminal text color to black. */
#define RED     "\033[31m" /**< Set terminal text color to red. */
#define GREEN   "\033[32m" /**< Set terminal text color to green. */
#define YELLOW  "\033[33m" /**< Set terminal text color to yellow. */
#define BLUE    "\033[34m" /**< Set terminal text color to blue. */
#define MAGENTA "\033[35m" /**< Set terminal text color to magenta. */
#define CYAN    "\033[36m" /**< Set terminal text color to cyan. */
#define WHITE   "\033[37m" /**< Set terminal text color to white. */

// Labels for server and client logs
#define SERVER "\033[33m[SERVER]\033[0m" /**< Label for server-side logs. */
#define CLIENT "\033[32m[CLIENT]\033[0m" /**< Label for client-side logs. */

// Labels for error and exception logs
#define ABN_ERROR "\033[31m[ERROR]\033[0m" /**< Label for error logs. */
#define ABN_EXCEPTION "\033[31m[EXCEPTION]\033[0m" /**< Label for exception logs. */

// Macros for logging with encapsulation and color
#define ABN_ENCAPSULATOR(function) BLUE << function << RESET /**< Encapsulate the function name in blue. */
#define ABN_DEBUG(source, function, description) std::cout << (source == SERVER_DEBUG ? SERVER : CLIENT) << ABN_ENCAPSULATOR(function) << " " << description << std::endl; /**< Print debug messages with source (server/client) and function name. */

// Enum for debug types
enum debug_type
{
    SERVER_DEBUG = 0, /**< Debug type for server logs. */
    CLIENT_DEBUG = 1, /**< Debug type for client logs. */
};

// Enum for verbosity levels
enum verbose_level
{
    basic = 0, /**< Basic verbosity level. */
    debug = 1, /**< Debug verbosity level. */
};

// Enum for command codes used in communication
enum command_code
{
    CONNECT = 0, /**< Command code for connection. */
    DISCONNECT = 1, /**< Command code for disconnection. */
};

namespace ABN {
    /**
     * @struct header_t
     * @brief Structure representing the header of a packet.
     *
     * This structure contains metadata for a network packet, including
     * the command code, client ID, and the size of the packet's body.
     */
    #pragma pack(push, 2)
    struct header_t
    {
        uint8_t magic[3]; /**< Magic string to identify the packet */
        uint8_t command_code_; /**< Command code indicating the type of packet. */
        uint16_t client_id_; /**< Unique client identifier. */
        uint16_t body_size_; /**< Size of the body section of the packet. */
    };

    /**
     * @struct packet_t
     * @brief Structure representing a network packet.
     *
     * A packet consists of a header and a body. The header provides
     * metadata about the packet, while the body contains the actual
     * data being transmitted.
     */
    struct packet_t
    {
        header_t header_; /**< Header of the packet containing metadata. */
        std::array<uint8_t, IPV4_MTU_HEADER_LESS> body_; /**< Body of the packet, containing the data. */
    };
    #pragma pack(pop)
}

/**
 * @brief Serializes the given data into a byte array.
 *
 * This function converts the provided data into a byte array of fixed size
 * (`IPV4_MTU_HEADER_LESS`), which can then be transmitted over the network.
 *
 * @param data A pointer to the data to serialize.
 * @param size The size of the data to serialize.
 * @return A byte array containing the serialized data.
 */
inline std::array<uint8_t, IPV4_MTU_HEADER_LESS> Serialization(const void *data, const uint16_t size)
{
    std::array<uint8_t, IPV4_MTU_HEADER_LESS> ary{0};

    std::memcpy(ary.data(), data, size);
    return ary;
}

inline bool CheckMagic(const uint8_t a[3])
{
    return (a[0] == 'A') && (a[1] == 'B') && (a[2] == 'N');
}

#endif /* MISC_HPP */
