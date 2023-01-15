#include <SDK/foobar2000.h>

DECLARE_COMPONENT_VERSION("Remove Playing Track", "1.2", "zao");
VALIDATE_COMPONENT_FILENAME("foo_np_remove.dll");

struct Commands : mainmenu_commands
{
    t_uint32 get_command_count() override { return 2; }

    GUID get_command(t_uint32 p_index) override
    {
        assert(p_index < 2);
        // {8237D1F2-252F-45BA-8FEA-60E7193C355D}
        static GUID const s_advance_next_guid = {
            0x8237d1f2, 0x252f, 0x45ba, { 0x8f, 0xea, 0x60, 0xe7, 0x19, 0x3c, 0x35, 0x5d }
        };
        // {ECDD0260-7E7D-40D0-AC77-D4C5F3F54A47}
        static GUID const s_advance_guid = {
            0xecdd0260, 0x7e7d, 0x40d0, { 0xac, 0x77, 0xd4, 0xc5, 0xf3, 0xf5, 0x4a, 0x47 }
        };
        if (p_index == 0) {
            return s_advance_next_guid;
        }
        return s_advance_guid;
    }

    void get_name(t_uint32 p_index, pfc::string_base& p_out) override
    {
        assert(p_index < 2);
        switch (p_index) {
            case 0:
                p_out = "Remove current track and advance playback";
                break;
            case 1:
                p_out = "Remove current track";
                break;
        }
    }

    bool get_description(t_uint32 p_index, pfc::string_base& p_out) override
    {
        assert(p_index < 2);
        switch (p_index) {
            case 0:
                p_out = "Remove currently playing track from list and advance playback.";
                break;
            case 1:
                p_out = "Remove currently playing track from list.";
                break;
        }
        return true;
    }

    GUID get_parent() override { return mainmenu_groups::edit; }

    void execute(t_uint32 p_index, service_ptr_t<service_base> p_callback) override
    {
        assert(p_index < 2);
        bool const also_advance = p_index == 0;
        static_api_ptr_t<playlist_manager_v4> pm;
        t_size playlist_index, item_index;
        if (pm->get_playing_item_location(&playlist_index, &item_index)) {
            bit_array_one arr(item_index);
            pm->playlist_undo_backup(playlist_index);
            pm->playlist_remove_items(playlist_index, arr);
            static_api_ptr_t<playback_control_v3> pc;
            if (also_advance) {
                pc->next();
            }
        }
    }
};

static mainmenu_commands_factory_t<Commands> g_commands;