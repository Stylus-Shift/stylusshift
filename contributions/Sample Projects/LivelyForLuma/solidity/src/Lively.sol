// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.13;

struct Stats {
    int8 overall;
    int8 food;
    int8 technical;
    int8 networking;
    int8 fun;
    int8 swag;
}

struct Event {
    string name;
    string host;
    string venue;
    string imageUrl;
    string lumaLink;
    string time;
    string long;
    string lat;
    Stats stats;
}

contract Lively {
    Event[] public events;

    // Check if a user has already rated event
    mapping (uint256 => mapping (address => bool)) public hasRated;

    // User's event prefrences
    mapping (address => string) public userPref;

    function createEvent(
        string memory _name,
        string memory _host,
        string memory _venue,
        string memory _imageUrl,
        string memory _lumaLink,
        string memory _time,
        string memory _long,
        string memory _lat,
        Stats memory _stats
    ) public {
        events.push(Event({
            name: _name,
            host: _host,
            venue: _venue,
            imageUrl: _imageUrl,
            lumaLink: _lumaLink,
            time: _time,
            long: _long,
            lat: _lat,
            stats: _stats
        }));
    }

    // Vote types: 1 for upvote, 2 for downvote, 0 for no vote
    function rateEvent(
        uint256 eventId,
        uint8 overallVote,
        uint8 foodVote,
        uint8 technicalVote,
        uint8 networkingVote,
        uint8 funVote,
        uint8 swagVote
    ) public {
        require(eventId < events.length, "Event does not exist");
        require(overallVote == 1 || overallVote == 2, "Overall vote is required");
        require(!hasRated[eventId][msg.sender], "User has already rated this event");

        // Verify user is checked into event in Luma (eg. they actually attended)
        verifyAttendance(eventId, msg.sender);

     
        
        // Update overall rating (required)
        if (overallVote == 1) {
            events[eventId].stats.overall += 1;
        } else {
            events[eventId].stats.overall -= 1;
        }
        
        // Update optional ratings
        if (foodVote != 0) {
            events[eventId].stats.food += (foodVote == 1) ? int8(1) : int8(-1);
        }
        if (technicalVote != 0) {
            events[eventId].stats.technical += (technicalVote == 1) ? int8(1) : int8(-1);
        }
        if (networkingVote != 0) {
            events[eventId].stats.networking += (networkingVote == 1) ? int8(1) : int8(-1);
        }
        if (funVote != 0) {
            events[eventId].stats.fun += (funVote == 1) ? int8(1) : int8(-1);
        }
        if (swagVote != 0) {
            events[eventId].stats.swag += (swagVote == 1) ? int8(1) : int8(-1);
        }

        hasRated[eventId][msg.sender] = true;
    }

    function setUserPref(string memory pref) public {
        userPref[msg.sender] = pref;
    }

    // Verify user is checked into event in Luma (eg. they actually attended). Placeholder. Assuming we had access to the Luma API which is $70/mo :(
    function verifyAttendance(uint256 eventId, address user) public returns (bool) {
        return true;
    }   

    function getAllEvents() public view returns (Event[] memory) {
        return events;
    }
}
