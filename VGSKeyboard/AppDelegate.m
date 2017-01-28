//
//  AppDelegate.m
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/28.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//

#import "AppDelegate.h"
#import "vgs2.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    vgs2_term();
}


@end
