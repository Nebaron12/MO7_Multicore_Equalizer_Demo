# 2025-06-23T08:23:42.502675700
import vitis

client = vitis.create_client()
client.set_workspace(path="SID_Multicore_Demo")

proj = client.get_sys_project(name="Multicore_Demo_System")

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

status = proj.clean()

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

comp = client.get_component(name="Audio_Core")
comp.build()

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

comp.build()

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj = client.get_sys_project(name="Multicore_Demo_System")

status = proj.import_files(from_loc="$COMPONENT_LOCATION/../UI_Core/src", files=["SharedMem"], dest_dir_in_cmp = "Multicore_Demo_System")

proj = client.get_sys_project(name="Multicore_Demo_System")

proj.build(comp_name = ["UI_Core"],build_comps = False)

proj.build(comp_name = ["UI_Core"],build_comps = False)

proj.build(comp_name = ["UI_Core"],build_comps = False)

comp = client.get_component(name="UI_Core")
comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

comp.build()

status = comp.clean()

comp.build()

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

proj.build(comp_name = ["UI_Core"],build_comps = False)

status = proj.clean()

proj.build(comp_name = ["UI_Core"],build_comps = False)

comp.build()

proj.build(comp_name = ["UI_Core"],build_comps = False)

proj.build(comp_name = ["UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

status = proj.clean()

status = proj.clean()

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

status = proj.clean()

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

comp.build()

comp = client.get_component(name="UI_Core")
comp.build()

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

status = proj.clean()

comp = client.get_component(name="Audio_Core")
status = comp.clean()

comp = client.get_component(name="UI_Core")
status = comp.clean()

proj.build(build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

vitis.dispose()

vitis.dispose()

