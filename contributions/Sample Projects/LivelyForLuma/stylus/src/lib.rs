#![cfg_attr(not(feature = "export-abi"), no_main)]
extern crate alloc;

use stylus_sdk::{
    alloy_primitives::{Address, U256, I8},
    prelude::*,
};

#[derive(PartialEq, Eq, Debug)]
pub struct Stats {
    overall: I8,
    food: I8,
    technical: I8,
    networking: I8,
    fun: I8,
    swag: I8,
}

#[derive(PartialEq, Eq, Debug)]
pub struct Event {
    name: String,
    host: String,
    venue: String,
    image_url: String,
    luma_link: String,
    time: String,
    long: String,
    lat: String,
    stats: Stats,
}

sol_storage! {
    #[entrypoint]
    pub struct Lively {
        Vec<Event> events;
        mapping(U256 => mapping(Address => bool)) has_rated;
        mapping(Address => String) user_pref;
    }
}

#[external]
impl Lively {
    pub fn create_event(
        &mut self,
        name: String,
        host: String,
        venue: String,
        image_url: String,
        luma_link: String,
        time: String,
        long: String,
        lat: String,
        stats: Stats,
    ) {
        let event = Event {
            name,
            host,
            venue,
            image_url,
            luma_link,
            time,
            long,
            lat,
            stats,
        };
        self.events.push(event);
    }

    pub fn rate_event(
        &mut self,
        event_id: U256,
        overall_vote: u8,
        food_vote: u8,
        technical_vote: u8,
        networking_vote: u8,
        fun_vote: u8,
        swag_vote: u8,
    ) {
        let sender = msg::sender();
        require!(event_id < U256::from(self.events.len()), "Event does not exist");
        require!(
            overall_vote == 1 || overall_vote == 2,
            "Overall vote is required"
        );
        require!(
            !self.has_rated.get(event_id).get(sender),
            "User has already rated this event"
        );

        self.verify_attendance(event_id, sender);

        let event = &mut self.events[event_id.as_usize()];
        
        // Update overall rating
        event.stats.overall += if overall_vote == 1 { I8::from(1) } else { I8::from(-1) };
        
        // Update optional ratings
        if food_vote != 0 {
            event.stats.food += if food_vote == 1 { I8::from(1) } else { I8::from(-1) };
        }
        if technical_vote != 0 {
            event.stats.technical += if technical_vote == 1 { I8::from(1) } else { I8::from(-1) };
        }
        if networking_vote != 0 {
            event.stats.networking += if networking_vote == 1 { I8::from(1) } else { I8::from(-1) };
        }
        if fun_vote != 0 {
            event.stats.fun += if fun_vote == 1 { I8::from(1) } else { I8::from(-1) };
        }
        if swag_vote != 0 {
            event.stats.swag += if swag_vote == 1 { I8::from(1) } else { I8::from(-1) };
        }

        self.has_rated.get(event_id).insert(sender, true);
    }

    pub fn set_user_pref(&mut self, pref: String) {
        self.user_pref.insert(msg::sender(), pref);
    }

    pub fn verify_attendance(&self, _event_id: U256, _user: Address) -> bool {
        true // Placeholder implementation
    }

    pub fn get_all_events(&self) -> Vec<Event> {
        self.events.clone()
    }

    pub fn events(&self, index: U256) -> Event {
        require!(index < U256::from(self.events.len()), "Index out of bounds");
        self.events[index.as_usize()].clone()
    }

    pub fn has_rated(&self, event_id: U256, user: Address) -> bool {
        self.has_rated.get(event_id).get(user)
    }

    pub fn user_pref(&self, user: Address) -> String {
        self.user_pref.get(user).unwrap_or_default()
    }
}