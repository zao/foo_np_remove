#include <SDK/foobar2000.h>

#pragma comment(lib, "foobar2000_component_client.lib")
#pragma comment(lib, "foobar2000_sdk.lib")
#pragma comment(lib, "pfc.lib")
#pragma comment(lib, "shared.lib")

DECLARE_COMPONENT_VERSION("foo_np_remove", "1.1", "zao");
VALIDATE_COMPONENT_FILENAME("foo_np_remove.dll");

struct Commands : mainmenu_commands {
  t_uint32 get_command_count() override { return 1; }

  GUID get_command(t_uint32 p_index) override {
    // {8237D1F2-252F-45BA-8FEA-60E7193C355D}
    static GUID const s_guid = {
        0x8237d1f2,
        0x252f,
        0x45ba,
        {0x8f, 0xea, 0x60, 0xe7, 0x19, 0x3c, 0x35, 0x5d}};
    return s_guid;
  }

  void get_name(t_uint32 p_index, pfc::string_base &p_out) override {
    p_out = "Remove current track";
  }

  bool get_description(t_uint32 p_index, pfc::string_base &p_out) override {
    p_out = "Remove currently playing track from list.";
    return true;
  }

  GUID get_parent() override { return mainmenu_groups::edit; }

  void execute(t_uint32 p_index,
               service_ptr_t<service_base> p_callback) override {
    static_api_ptr_t<playlist_manager_v4> pm;
    t_size playlist_index, item_index;
    if (pm->get_playing_item_location(&playlist_index, &item_index)) {
      bit_array_one arr(item_index);
      pm->playlist_undo_backup(playlist_index);
      pm->playlist_remove_items(playlist_index, arr);
      static_api_ptr_t<playback_control_v3> pc;
      pc->next();
    }
  }
};

static mainmenu_commands_factory_t<Commands> g_commands;