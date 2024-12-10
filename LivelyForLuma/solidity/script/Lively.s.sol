// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.13;

import {Script, console} from "forge-std/Script.sol";
import {Lively, Event, Stats} from "../src/Lively.sol";
import {Vm} from "forge-std/Vm.sol";

contract DeployLively is Script {
    // Custom struct for JSON parsing that excludes stats

    Lively public lively;

    function setUp() public {}

    function run() public returns (Lively) {
        vm.startBroadcast();

        lively = new Lively();
     
        Event[] memory events = getEvents();

        for (uint i = 0; i < events.length; i++) {
            Event memory event_ = events[i];
            lively.createEvent(
                event_.name,
                event_.host,
                event_.venue,
                event_.imageUrl,
                event_.lumaLink,
                event_.time,
                event_.long,
                event_.lat,
                event_.stats
            );
        }

        vm.stopBroadcast();

        return lively;
    }

    // Simulate reading from Luma API, which costs $70 per month :(
    function getEvents() public pure returns (Event[] memory) {
        Event[] memory events = new Event[](15);

        events[0] = Event({
    name: "DEVCON RAVE",
    host: "Akshat Vaidya & Maelstrom Events",
    venue: "Bangkok",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/9i/047b0789-2845-4779-b467-7f98415582dd",
    lumaLink: "https://lu.ma/nu3rrwlh?tk=ESj6Ll",
    time: "10:00 PM - 3:30 AM",
    long: "100.51352391261277",
    lat: "13.725572965216754",
    stats: Stats({
        overall: 9,
        food: 7,
        technical: 4,
        networking: 6,
        fun: 10,
        swag: 8
    })
});

events[1] = Event({
    name: "BERAVE Bangkok - Beraborrow, $NUMBER with Beratone and Grand Conquest",
    host: "Tez & 6 others",
    venue: "BEAMCUBE",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/d3/9a0d4125-9770-4665-b392-95fba044840b",
    lumaLink: "https://lu.ma/1t9luav8?tk=GfeKsw",
    time: "9:30 PM - 2:30 AM",
    long: "100.52879722458857",
    lat: "13.72402611250967",
    stats: Stats({
        overall: 8,
        food: 6,
        technical: 3,
        networking: 8,
        fun: 9,
        swag: 6
    })
});

events[2] = Event({
    name: "Crypto Pool Party by JustMoney x Twiskers",
    host: "JustMoney & Twiskers",
    venue: "THE PIMP CLUB BANGKOK",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/c9/1c6a1daa-9c98-46a3-9865-2cae494e47a8",
    lumaLink: "https://lu.ma/cgrfaquz?tk=HI7a3h",
    time: "8:00 PM - 2:00 AM",
    long: "100.60179025706364",
    lat: "13.76785838518615",
    stats: Stats({
        overall: 7,
        food: 9,
        technical: 2,
        networking: 8,
        fun: 10,
        swag: 6
    })
});

events[3] = Event({
    name: "Frens of Aztec Cocktail Party",
    host: "Aztec Labs",
    venue: "The Gardens of Dinsor Palace",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/m2/2b12f5b4-ce05-43fd-98d1-26ec1c887126",
    lumaLink: "https://lu.ma/klk4hj2t?tk=I8XfG0",
    time: "7:00 PM - 11:00 PM",
    long: "100.58292593011394",
    lat: "13.722950922552196",
    stats: Stats({
        overall: 9,
        food: 10,
        technical: 3,
        networking: 10,
        fun: 8,
        swag: 5
    })
});

events[4] = Event({
    name: "Mitosis Game Night - Thailand Local Community Party",
    host: "Mitosis & Timeintime",
    venue: "Hard Rock Cafe Bangkok",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/gc/0644bb3e-bcda-4677-b1ac-e413105f4456",
    lumaLink: "https://lu.ma/3ax8ptou?tk=y60WpD",
    time: "7:00 PM - 11:00 PM",
    long: "100.54262468039788",
    lat: "13.743591546535841",
    stats: Stats({
        overall: 7,
        food: 7,
        technical: 5,
        networking: 7,
        fun: 7,
        swag: 9
    })
});

events[5] = Event({
    name: "Super totally normal cruise",
    host: "Reffo & 4 others",
    venue: "Oriental pier",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/4v/6b147b0e-d610-451c-bcb0-143fd2b66178",
    lumaLink: "https://lu.ma/puffdegn?tk=F19IDv",
    time: "7:00 PM - 10:00 PM",
    long: "100.51395905156265",
    lat: "13.723450605977728",
    stats: Stats({
        overall: 8,
        food: 5,
        technical: 2,
        networking: 9,
        fun: 10,
        swag: 6
    })
});

events[6] = Event({
    name: "Waterloo Alumni Rooftop Meetup",
    host: "Optimus",
    venue: "Arun Old Town Hostel",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/z8/ca90e943-968d-415a-8d6f-406d8cdd4423",
    lumaLink: "https://lu.ma/emwcojsw?tk=HRBNrd",
    time: "7:00 PM - 8:30 PM",
    long: "100.48508382272743",
    lat: "13.739508655300316",
    stats: Stats({
        overall: 6,
        food: 4,
        technical: 3,
        networking: 7,
        fun: 5,
        swag: 3
    })
});

events[7] = Event({
    name: "DeFi Night @ Devcon",
    host: "ETH Belgrade",
    venue: "Bangkok Art Biennale Cafe (The PARQ)",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/my/76b6cc2f-6166-4f46-9451-1d3a5f38c378",
    lumaLink: "https://lu.ma/mn1795k1?tk=0ecXz8",
    time: "6:30 PM - 11:00 PM",
    long: "100.55875751292636",
    lat: "13.721695662108965",
    stats: Stats({
        overall: 8,
        food: 6,
        technical: 10,
        networking: 9,
        fun: 7,
        swag: 5
    })
});

events[8] = Event({
    name: "GAS PARTY ELITE CLUB",
    host: "Pylynn & 3 others",
    venue: "Space plus bangkok",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/ck/074e4d23-fe66-4431-b9be-5b35f7963cc5",
    lumaLink: "https://lu.ma/20n09gvr?tk=9BiPRq",
    time: "5:30 PM - 8:00 PM",
    long: "100.58057305156264",
    lat: "13.747178900977833",
    stats: Stats({
        overall: 9,
        food: 8,
        technical: 3,
        networking: 6,
        fun: 9,
        swag: 7
    })
});

events[9] = Event({
    name: "Somnia's Uniting Societies @Devcon, Mai 'Thai' Mixer w/Somnia & Friends",
    host: "Somnia",
    venue: "ABar Rooftop",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/u7/e45a2f6f-a623-48cd-9c8e-87610abfe50c",
    lumaLink: "https://lu.ma/jeih46pm?tk=TaQEKf",
    time: "5:00 PM - 9:00 PM",
    long: "100.56583786690359",
    lat: "13.730464944862113",
    stats: Stats({
        overall: 10,
        food: 10,
        technical: 5,
        networking: 9,
        fun: 8,
        swag: 7
    })
});

events[10] = Event({
    name: "Green Lung Pool Party",
    host: "V DAO",
    venue: "Lory home",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/xv/cdf4791e-5613-4395-b7ea-ebbb8aeaebbb",
    lumaLink: "https://lu.ma/zyafugon?tk=Cww2aw",
    time: "3:00 PM - 11:00 PM",
    long: "100.58565485767045",
    lat: "13.68713748331647",
    stats: Stats({
        overall: 8,
        food: 6,
        technical: 4,
        networking: 9,
        fun: 8,
        swag: 8
    })
});

events[11] = Event({
    name: "Linea x Bitkub: UNBOUND",
    host: "Bitkub & Consensys",
    venue: "ZillaSpace",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/r2/e64c1d60-9fbb-4f9f-9c2f-4cae10f3609d",
    lumaLink: "https://lu.ma/7zgt2on0?tk=GLRmNL",
    time: "3:00 PM - 7:00 PM",
    long: "100.56023940553986",
    lat: "13.7203081269205",
    stats: Stats({
        overall: 9,
        food: 8,
        technical: 9,
        networking: 7,
        fun: 7,
        swag: 6
    })
});

events[12] = Event({
    name: "[ETHGlobal] Omi AI Hacker Hangout - Win a Dev Kit.",
    host: "Devcon",
    venue: "Marie Guimar",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/v7/6e1516f5-e483-4e6e-a4e7-c19a2cb94a3e",
    lumaLink: "https://lu.ma/moodeng?tk=tvuQ1b",
    time: "12:00 PM - 4:00 PM",
    long: "100.56137489204558",
    lat: "13.72340216089611",
    stats: Stats({
        overall: 8,
        food: 5,
        technical: 10,
        networking: 9,
        fun: 6,
        swag: 6
    })
});

events[13] = Event({
    name: "Climb & Cuddle: Real Capybara + Capy Kilter Board ($CAPY Airdrop)",
    host: "CapyLabs & 10 others",
    venue: "Magnus Climbing Gym World's First 24-Hour, Capybara-Themed, Air-Conditioned Climbing Experience",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/12/a7c7d540-de43-43e5-8f75-042a8934e948",
    lumaLink: "https://lu.ma/me473276?tk=Iy5rw8",
    time: "10:00 AM - 1:00 PM",
    long: "100.53129590312531",
    lat: "13.723077480548623",
    stats: Stats({
        overall: 10,
        food: 8,
        technical: 6,
        networking: 10,
        fun: 9,
        swag: 8
    })
});

events[14] = Event({
    name: "Friday at the Cafe",
    host: "Starknet Foundation",
    venue: "The Fig Lobby Bangkok",
    imageUrl: "https://images.lumacdn.com/cdn-cgi/image/format=auto,fit=cover,dpr=2,background=white,quality=75,width=300,height=300/event-covers/a1/61fab1da-180d-477e-adc2-867be87b28fb",
    lumaLink: "https://lu.ma/bu0gq5sp?tk=b7joBA",
    time: "10:00 AM - 4:00 PM",
    long: "100.56424702457404",
    lat: "13.718559860448524",
    stats: Stats({
        overall: 7,
        food: 6,
        technical: 9,
        networking: 8,
        fun: 6,
        swag: 7
    })
});


        return events;
    }
}

