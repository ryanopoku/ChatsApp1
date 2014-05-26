//
//  SCDataManager.h
//  C2CallPhone
//
//  Created by Michael Knecht on 02.06.13.
//  Copyright 2013 C2Call GmbH. All rights reserved.
//
//

#import <Foundation/Foundation.h>
#import "MOC2CallUser.h"
#import "MOC2CallEvent.h"
#import "MOC2CallGroup.h"
#import "MOChatHistory.h"
#import "MOCallHistory.h"

/** SCDataManager provides the low-level API access to the CoreData Database handled by C2Call SDK.
 
 The Database Schema is available under SocialCommunication.ressources/C2CallDataModel.xcdatamodeld.
 
 All relevant Entities in this Data Model are subclasses from NSManagedObject for convenient access to the Entity Attributes.
 The following NSManagedObject subclasses are defined by C2Call SDK.
 
    - MOChatHistory
    - MOC2CallUser
    - MOCallHistory
    - MOC2CallEvent
    - MOOpenId
    - MOAddress
    - MOPhoneNumber
    - MOGroupMember
    - MOC2CallGroup
 
 C2Call SDK GUI Components are based on CoreData.
 All access to CoreData Objects and all queries to the database have to be done in main Thread.
 SCDataManager capsules the access to the CoreData database layer and ensures that all request are done in main Thread.
 
 */
@interface SCDataManager : NSObject

/** Gets the MOC2CallUser Object for a userId.
 
 The MOC2CallUser Object represents a friend, a group or a special user (CallMe Link, Test Call, etc).
 All relevant information on those users is available through MOC2CallUser Object. The data is read only; changes to the object will be discarded on next data synchronisation with the server.
 
 @param userid - C2Call Userid 
 @return The user object or nil
 */
-(MOC2CallUser *) userForUserid:(NSString *) userid;

/** Gets the MOC2CallUser Object for an email address.
 The MOC2CallUser Object represents a friend, a group or a special user (CallMe Link, Test Call, etc).
 All relevant information on those users is available through MOC2CallUser Object. The data is read only; changes to the object will be discarded on next data synchronisation with the server.
 
 @param email - Email address of the user object
 @return The user object or nil
 */
-(MOC2CallUser *) userForEmail:(NSString *) email;

/** Gets the MOC2CallEvent Object for an eventId.
 
 The MOC2CallEvent Object represents a call or a message, which can be inbound call, outbound call, inbound message, outbound message.
 All relevant information on those calls or messages is available through MOC2CallEvent Object. The data is read only; changes to the object will be discarded on next data synchronisation with the server.
 
 @param eventId - EventId of the object
 @return The event object or nil
 */
-(MOC2CallEvent *) eventForEventId:(NSString *) eventId;

/** Gets the MOC2CallGroup Object for a groupid.
 
 The MOC2CallGroup Object represents a group.
 All relevant information on the group is available through MOC2CallGroup Object. The data is read only; changes to the object will be discarded on next data synchronisation with the server.
 
 @param eventId - EventId of the object
 @return The event object or nil
 */
-(MOC2CallGroup *) groupForGroupid:(NSString *) groupid;

/** Mark an MOC2CallEvent as read
 
 MOC2CallEvent messages of type MessageIn can be marked as read.
 A read notification will be submitted to the remote party.
 The Missed Events counter will be decremented.
 
 @param event - The MOC2CallEvent to be marked as read
 @return YES : In MOC2CallEvent is of type MessageIn and status is unread else NO
 */

-(BOOL) markAsRead:(MOC2CallEvent *) event;

/** Get the number of total missed messages

 @return Number of missed messages
*/
-(int) totalMissedMessages;

/** Get the number of total missed calls
 
 @return Number of missed calls
 */
-(int) totalMissedCalls;

/** Get the number of missed calls for a specific contact
 
 @param contact - Userid of the contact.
 @return Number of missed calls
 */
-(int) missedCallsForContact:(NSString *) contact;

/** Manually reset the missed calls indication for a contact
 
 @param contact - Userid of the contact. 
 */
-(void) resetMissedCallsForContact:(NSString *) contact;

/** Removes a NSManagedObject from the Database.
 
 The removeDatabaseObject Method removes the following NSManagedObject subclasses correctly from the database and from the server:
 
    - MOC2CallEvent
    - MOC2CallUser
    - MOChatHistory
    - MOCallHistory
 
 The above-mentioned objects have a representation locally on the device and as well on the server with several dependencies to other objects. 
 Therefore deleting an object is a complex task, which has to be handled correctly. 
 For example: 
    The MOC2CallUser Object represents either friend relation or a group (in case the use owns the group) or a group relation (the user is only member of a group).
    Deleting an MOC2CallUser object has therefore the following implications:
        1. In case the MOC2CallUser represents a friend relation, the relation will be removed (this two users are not longer friends)
        2. In case the MOC2CallUser represents a group (the user owns the group), the actual group will be deleted on the server and all memberships will be released.
        3. In case the MOC2CallUser represents a group relation, only the membership of this user will be released.
 
    The MOC2CallEvent represents an instant message, a rich media message or a call record.
    In case of an instant message or a call record, removing the MOC2CallEvent object only removes the corresponding record on the server.
    In case of a rich media message, all related media files will be removed, too.
 
    The MOChatHistory is a container object for all messages with a specific person.
    Removing a MOChatHistory object will automatically remove all MOC2CallEvent objects from and to that person, too.
 
    The MOCallHistory is a container object for all calls with a specific person.
    Removing a MOCallHistory object will automatically remove all MOC2CallEvent objects from and to that person, too.
 
 @param managedObject - Either one of the descibed objects above.
 @return YES - success / NO - failure
 */
-(BOOL) removeDatabaseObject:(NSManagedObject *) managedObject;

/** Gets a predefined NSFetchRequest for a Chat History.
 
 @see SCDataTableViewController and WhazzUpp Sample Code for more details on how to use NSFetchRequests
 
 @param sortAscending YES - Fetch MOChatHistory objects in the order of the oldest chat first / NO - Latest Chat first
 */
-(NSFetchRequest *) fetchRequestForChatHistory:(BOOL) sortAscending;

/** Gets a predefined NSFetchRequest for a Call History.
 
 @see SCDataTableViewController and WhazzUpp Sample Code for more details on how to use NSFetchRequests

 @param sortAscending YES - Fetch MOCallHistory objects in the order of the oldest call first / NO - Latest call first
*/
-(NSFetchRequest *) fetchRequestForCallHistory:(BOOL) sortAscending;

/** Gets a predefined NSFetchRequest for a list of MOC2CallEvent objects of the specific contact.
 
 @see SCDataTableViewController and WhazzUpp Sample Code for more details on how to use NSFetchRequests
 
 @param sortAscending YES - Oldest Event first / NO - Latest Event first
 */
-(NSFetchRequest *) fetchRequestForEventHistory:(NSString *) userid sort:(BOOL) sortAscending;


/** Gets a predefined NSFetchRequest for a list of MOC2CallUser objects (list of all friends and groups).
 
 @see SCDataTableViewController and WhazzUpp Sample Code for more details on how to use NSFetchRequests
 
 @param sortByFirstname YES - Sort by Firstname / NO - Sort by Lastname
 */
-(NSFetchRequest *) fetchRequestForFriendlist:(BOOL) sortByFirstname;


/** Sets a fetch size limit on a given fetch request.
 
 The result set of a fetch request can be quite large, on a long chat for example. 
 Setting the FetchLimit limits the result set to the last fetchLimit objects (for example the last 25 messages of a chat).
 
    NSFetchRequest *fetchRequest = [[SCDataManager instance] fetchRequestForEventHistory:@"<Userid of Chat Partner>" sort:YES];
    [[SCDataManager instance] setFetchLimit:25 forFetchRequest:fetchRequest];
 
 @see SCDataTableViewController and WhazzUpp Sample Code for more details on how to use NSFetchRequests
 
 @param fetchLimit - Max number of objects in the result set
 @param fetchRequest - The fetchRequest to limit
 
 */
-(int) setFetchLimit:(int) fetchLimit forFetchRequest:(NSFetchRequest *) fetchRequest;

/** Requests a NSFetchedResultsController for the C2Call SDK Database with a given fetchRequest.
 
 @see SCDataTableViewController and WhazzUpp Sample Code for more details on how to use NSFetchedResultsController
 
 @param fetchRequest - The fetchRequest
 @param sectionNameKeypath - A key path on result objects that returns the section name. Pass nil to indicate that the controller should generate a single section.
 The section name is used to pre-compute the section information.
 If this key path is not the same as that specified by the first sort descriptor in fetchRequest, they must generate the same relative orderings. For example, the first sort descriptor in fetchRequest might specify the key for a persistent property; sectionNameKeyPath might specify a key for a transient property derived from the persistent property.
 @param cacheName - The name of the cache file the receiver should use. Pass nil to prevent caching.
 Pre-computed section info is cached to a private directory under this name. If Core Data finds a cache stored with this name, it is checked to see if it matches the fetchRequest. If it does, the cache is loaded directly—this avoids the overhead of computing the section and index information. If the cached information doesn’t match the request, the cache is deleted and recomputed when the fetch happens.
 
 */
-(NSFetchedResultsController *) fetchedResultsControllerWithFetchRequest:(NSFetchRequest *)fetchRequest sectionNameKeyPath:(NSString *)sectionNameKeyPath cacheName:(NSString *)name;

/** Current Instance of SCDataManager.
 
 @return Current Instance of SCDataManager
 */
+(SCDataManager *) instance;

/** Is CoreData initialized already.
 
 @return YES - Initialized / NO - Not initialized yet.
 */
@property(nonatomic) BOOL       isDataInitialized;

@end
