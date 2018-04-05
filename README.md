# PowerEFI_Smartfusion_Programmer
In system programmer firmware used to load new firmware and update FPGA fabric.

### Building sources

# TODO create Bazel project for CortexM3:

### Pushing Code For Review

Gerrithub is a central location used for easy managment of the code review process. To get an idea of how this is typically done, watch the following video and pay special attention to the 2:25+ mark:

[![Video](http://img.youtube.com/vi/jeWTvDad6VM/0.jpg)](http://www.youtube.com/watch?v=jeWTvDad6VM)

Simply perform your changes and [submit for review](https://review.gerrithub.io/Documentation/intro-project-owner.html#code-review):

```bash
git push ssh://seank@gerrithub.io:29418/seank/PowerEFI_SmartFusion_Programmer HEAD:refs/for/master
```

#NOTE
--change-section-lma *-0x60000000 for release images so that the envclient does not reject the image,
see softconsole-v4-0 release notes
