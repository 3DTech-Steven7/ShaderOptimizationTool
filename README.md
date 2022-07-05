# ShaderOptimizationTool 场景材质优化工具

## what is this plugin 这是什么插件

This plugin can help you improved scene shader process.
这个插件可以帮助你改进场景材质优化流程。

## Do you more or less encounter this problem? 你是否或多或少遇到这样的问题

- On Tuesday, our members will compile and compile again after modification, the previous compilation will not be cancelled, and the development can continue. We will wait for the compilation to complete or continue.
    通常，我们材质修改后编译再次修改编译后，不会撤销之前编译队列，而是会继续叠加，我们只能等待编译完成或者重开。

- When we open the scene, thousands of material combinations will be triggered. Maybe we don't need material combinations. We just open the scene and edit the level blueprint or something.
    我们打开场景时，会触发上千上万的材质编译，可能我们不需要材质编译，我们只是打开场景编辑一下关卡蓝图什么的。

- One, and want to see both the scene trigger the material compilation, but just want to see the material model.
    拖放一个Mesh到场景都触发材质编译，但只是想放置模型，并不想看到材质。

This is a simple and easy-to-use plug-in that can be used with just one click, and has four functions
那么有一款插件能解决以上问题岂不是很妙，那就先看看功能效果。

## Features 特性

### SkipShaderCompilation 跳过材质编译

- Can skip scene material compilation, but do not open the material editor after enabling the skip material compilation function, and do not compile it if it is already open
- 可以跳过材质编译，不过注意开启跳过材质编译功能后不要打开材质编辑器，已经打开也不要编译

### Switch Lembert View 切换灰模显示

- Turn on to turn the scene into a grayscale effect, turn it off to restore the default display effect, and the selected object will not turn into a grayscale effect
- 开启将场景变为灰模效果，关闭则恢复默认显示效果，选择的物体不会变成灰模效果

### CancelAllCompilations 关闭当前未编译队列

- Close the current shader compilation queue
- 关闭当前材质编译的队列

### ReCompilations 重编译

- You can select or recompile the shader for the entire scene
- 可以选择或者将整个场景的材质重新编译