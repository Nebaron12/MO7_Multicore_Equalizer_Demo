# 2025-07-02T19:03:18.560395900
import vitis

client = vitis.create_client()
client.set_workspace(path="SID_Multicore_Demo")

proj = client.get_sys_project(name="Multicore_Demo_System")

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

proj.build(comp_name = ["Audio_Core", "UI_Core"],build_comps = False)

