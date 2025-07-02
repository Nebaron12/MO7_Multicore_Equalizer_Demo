# 2025-06-21T22:43:30.228167800
import vitis

client = vitis.create_client()
client.set_workspace(path="SID_Multicore_Demo")

advanced_options = client.create_advanced_options_dict(dt_overlay="0")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../../Embedded/Custom_Platforms/Custom_Platform_Ultrasone_1_4.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",generate_dtb = False,advanced_options = advanced_options,compiler = "gcc")

platform = client.get_component(name="platform")
status = platform.build()

proj = client.create_sys_project(name="Multicore_Demo_System", platform="$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm", template="empty_accelerated_application" , build_output_type="xsa")

domain = platform.add_domain(cpu = "ps7_cortexa9_1",os = "standalone",name = "standalone_ps7_cortexa9_1",display_name = "standalone_ps7_cortexa9_1",generate_dtb = False)

comp = client.create_app_component(name="UI_Core",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_1")

comp = client.create_app_component(name="Audio_Core",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")

proj = client.get_sys_project(name="Multicore_Demo_System")

proj = client.get_sys_project(name="Multicore_Demo_System")

proj = proj.add_component(name="Audio_Core")

proj = proj.add_component(name="UI_Core")

comp = client.get_component(name="UI_Core")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../SID_Workspace/SID_Equalizer_V2/src", files=["FaderPanel", "I2C", "UART"], dest_dir_in_cmp = "src")

status = platform.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../SID_Workspace/SID_Equalizer_V2/src/EQ", files=["EQ_Controls.c", "EQ_Controls.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../SID_Workspace/SID_Equalizer_V2/src/EQ", files=["EQ_Controls.c", "EQ_Controls.h"], dest_dir_in_cmp = "EQControls")

comp = client.get_component(name="UI_Core")
comp.build()

comp.build()

comp = client.get_component(name="UI_Core")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../MO7_Workspace/UI_Manager", files=["I2Csrc"], dest_dir_in_cmp = "OLED")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../MO7_Workspace/UI_Manager/src", files=["OLED.c", "OLED.h"], dest_dir_in_cmp = "OLED")

comp = client.get_component(name="UI_Core")
comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp = client.get_component(name="Audio_Core")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../SID_Workspace/SID_Equalizer_V2/src", files=["Main.c"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../SID_Workspace/SID_Equalizer_V2/src/EQ", files=["EQ_Controls.c", "EQ_Controls.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../UI_Core/src", files=["SharedMem"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../SID_Workspace/SID_Equalizer_V2/src", files=["Codec", "EQ"], dest_dir_in_cmp = "src")

comp = client.get_component(name="UI_Core")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../Audio_Core/src", files=["Codec"], dest_dir_in_cmp = "src")

comp = client.get_component(name="UI_Core")
comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp = client.get_component(name="Audio_Core")
comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

proj = client.get_sys_project(name="Multicore_Demo_System")

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

vitis.dispose()

