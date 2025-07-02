# 2025-06-24T12:50:15.769974100
import vitis

client = vitis.create_client()
client.set_workspace(path="SID_Multicore_Demo")

status = client.export_projects(components = ["Audio_Core", "platform", "UI_Core"], system_projects = ["Multicore_Demo_System"], include_build_dir = False, dest = "C:\Users\dbake\Downloads\SID_Multicore_Demo_Export.zip")

