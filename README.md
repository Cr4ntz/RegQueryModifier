# RegQueryModifier
Intercepts calls made from applications that queries the registry. By doing this you can change what values are read to modify application behaviour. In this case an application attempts to check if a proxy is running on the system by querying a registry key. The call is intercepted and the value read is changed to indicate that no proxy is present, even if one is. 
