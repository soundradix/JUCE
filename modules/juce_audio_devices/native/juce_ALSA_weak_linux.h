/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

// This header holds definitions for ALSA symbols that are only present in newer library versions.
// It also contains weak definitions for functions that are only present in newer library versions.
// By requesting the functions to weak-link, it's possible to build UMP-aware programs on Linux
// versions that don't have up-to-date copies of ALSA, and it's also possible to run software
// built on older Linux platforms, even if that software was built on a more recent system.

#if SND_LIB_VERSION < ((1 << 16) | (2 << 8) | 10)

struct snd_seq_ump_event_t
{
    snd_seq_event_type_t type;
    unsigned char flags;
    unsigned char tag;
    unsigned char queue;
    snd_seq_timestamp_t time;
    snd_seq_addr_t source;
    snd_seq_addr_t dest;

    union
    {
        decltype (snd_seq_event_t::data) data; // Older headers don't define snd_seq_event_data_t
        unsigned int ump[4];
    };
};

struct snd_ump_endpoint_info_t;
struct snd_ump_block_info_t;

#define snd_ump_block_info_alloca(ptr) __snd_alloca (ptr, snd_ump_block_info)
#define snd_ump_endpoint_info_alloca(ptr) __snd_alloca (ptr, snd_ump_endpoint_info)

constexpr auto SND_SEQ_CLIENT_LEGACY_MIDI = 0;
constexpr auto SND_SEQ_CLIENT_UMP_MIDI_1_0 = 1;
constexpr auto SND_SEQ_CLIENT_UMP_MIDI_2_0 = 2;

constexpr auto SND_UMP_EP_INFO_STATIC_BLOCKS = 0x1;
constexpr auto SND_UMP_EP_INFO_PROTO_MIDI1 = 0x100;
constexpr auto SND_UMP_EP_INFO_PROTO_MIDI2 = 0x200;

constexpr auto SND_SEQ_PORT_CAP_INACTIVE = 1 << 8;

constexpr auto SND_SEQ_EVENT_UMP = 1 << 5;

constexpr auto SND_SEQ_PORT_DIR_INPUT = 1;
constexpr auto SND_SEQ_PORT_DIR_OUTPUT = 2;
constexpr auto SND_SEQ_PORT_DIR_BIDIRECTION = 3;

constexpr auto SND_UMP_BLOCK_UI_HINT_UNKNOWN = 0;
constexpr auto SND_UMP_BLOCK_UI_HINT_RECEIVER = 1;
constexpr auto SND_UMP_BLOCK_UI_HINT_SENDER = 2;
constexpr auto SND_UMP_BLOCK_UI_HINT_BOTH = 3;

constexpr auto SND_UMP_BLOCK_IS_MIDI1 = 1U << 0;
constexpr auto SND_UMP_BLOCK_IS_LOWSPEED = 1U << 1;

constexpr auto SND_UMP_DIR_INPUT = 1;
constexpr auto SND_UMP_DIR_OUTPUT = 2;
constexpr auto SND_UMP_DIR_BIDIRECTION = 3;

constexpr auto SND_UMP_EP_INFO_PROTO_JRTS_TX = 1;
constexpr auto SND_UMP_EP_INFO_PROTO_JRTS_RX = 2;
#endif

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wredundant-decls")

extern "C"
{
    [[gnu::weak]] int snd_seq_client_info_get_midi_version (const snd_seq_client_info_t*);
    [[gnu::weak]] int snd_seq_set_client_midi_version (snd_seq_t*, int);
    [[gnu::weak]] int snd_seq_set_client_ump_conversion (snd_seq_t*, int);
    [[gnu::weak]] int snd_seq_ump_event_input (snd_seq_t*, snd_seq_ump_event_t**);
    [[gnu::weak]] int snd_seq_ump_event_output_direct (snd_seq_t*, snd_seq_ump_event_t*);
    [[gnu::weak]] int snd_seq_port_info_get_ump_is_midi1 (const snd_seq_port_info_t*);
    [[gnu::weak]] int snd_seq_port_info_get_ump_group (const snd_seq_port_info_t*);
    [[gnu::weak]] int snd_seq_create_ump_endpoint (snd_seq_t*, const snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] int snd_seq_create_ump_block (snd_seq_t*, int, const snd_ump_block_info_t*);
    [[gnu::weak]] int snd_seq_set_ump_block_info (snd_seq_t*, int, const void*);
    [[gnu::weak]] int snd_seq_get_ump_block_info (snd_seq_t*, int, int, void*);
    [[gnu::weak]] int snd_seq_get_ump_endpoint_info (snd_seq_t*, int, void*);

    [[gnu::weak]] const char* snd_ump_endpoint_info_get_product_id (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] const unsigned char* snd_ump_endpoint_info_get_sw_revision (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_family_id (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_flags (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_manufacturer_id (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_model_id (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_num_blocks (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_protocol (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_protocol_caps (const snd_ump_endpoint_info_t*);
    [[gnu::weak]] unsigned int snd_ump_endpoint_info_get_version (const snd_ump_endpoint_info_t*);

    [[gnu::weak]] void snd_ump_endpoint_info_set_product_id (snd_ump_endpoint_info_t*, const char*);
    [[gnu::weak]] void snd_ump_endpoint_info_set_sw_revision (snd_ump_endpoint_info_t*, const unsigned char*);
    [[gnu::weak]] void snd_ump_endpoint_info_set_family_id (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_flags (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_manufacturer_id (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_model_id (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_name (snd_ump_endpoint_info_t*, const char*);
    [[gnu::weak]] void snd_ump_endpoint_info_set_num_blocks (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_protocol (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_protocol_caps (snd_ump_endpoint_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_endpoint_info_set_version (snd_ump_endpoint_info_t*, unsigned int);

    [[gnu::weak]] const char* snd_ump_block_info_get_name (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_active (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_direction (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_first_group (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_flags (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_num_groups (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_sysex8_streams (const snd_ump_block_info_t*);
    [[gnu::weak]] unsigned int snd_ump_block_info_get_ui_hint (const snd_ump_block_info_t*);

    [[gnu::weak]] void snd_ump_block_info_set_name (snd_ump_block_info_t*, const char*);
    [[gnu::weak]] void snd_ump_block_info_set_active (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_direction (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_first_group (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_flags (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_num_groups (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_sysex8_streams (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_ui_hint (snd_ump_block_info_t*, unsigned int);
    [[gnu::weak]] void snd_ump_block_info_set_block_id (snd_ump_block_info_t*, unsigned int);

    [[gnu::weak]] int snd_seq_port_info_get_direction (const snd_seq_port_info_t*);

    [[gnu::weak]] size_t snd_ump_block_info_sizeof (void);
    [[gnu::weak]] size_t snd_ump_endpoint_info_sizeof (void);
}
