# 2025-06-23T22:39:56.105193600
import vitis

client = vitis.create_client()
client.set_workspace(path="SID_Multicore_Demo")

comp = client.get_component(name="Audio_Core")
status = comp.clean()

comp = client.get_component(name="UI_Core")
status = comp.clean()

proj = client.get_sys_project(name="Multicore_Demo_System")

proj.build(build_comps = False)

status = proj.clean()

comp = client.get_component(name="Audio_Core")
status = comp.clean()

comp = client.get_component(name="UI_Core")
status = comp.clean()

proj.build(build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

comp = client.get_component(name="Audio_Core")
status = comp.clean()

comp = client.get_component(name="UI_Core")
status = comp.clean()

status = proj.clean()

proj.build(build_comps = False)

platform = client.get_component(name="platform")
status = platform.build()

proj.build(build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

comp = client.get_component(name="Audio_Core")
status = comp.clean()

comp = client.get_component(name="UI_Core")
status = comp.clean()

status = proj.clean()

proj.build(build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

vitis.dispose()

