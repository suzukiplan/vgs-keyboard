//
//  ViewController.m
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/28.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//

#import "ViewController.h"
#import "MIKMIDI.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    MIKMIDIDeviceManager* dm = [MIKMIDIDeviceManager sharedDeviceManager];
    NSArray<MIKMIDIDevice*>* devices = [dm availableDevices];
    for (MIKMIDIDevice* device in devices) {
        NSArray<MIKMIDIEntity*>* entities = device.entities;
        for (MIKMIDIEntity* entity in entities) {
            NSArray<MIKMIDISourceEndpoint*>* sources = entity.sources;
            for (MIKMIDISourceEndpoint* source in sources) {
                NSLog(@"source: %@", source);
                [dm connectInput:source error:nil eventHandler:^(MIKMIDISourceEndpoint * _Nonnull source, NSArray<MIKMIDICommand *> * _Nonnull commands) {
                    NSLog(@"detected commands: %@", commands);
                }];
            }
            NSArray<MIKMIDIDestinationEndpoint*>* destinations = entity.destinations;
            for (MIKMIDIDestinationEndpoint* destination in destinations) {
                NSLog(@"destination: %@", destination);
            }
        }
    }
}

- (void)didRespond {
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
